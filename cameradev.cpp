#include "cameradev.h"

#include <QDebug>

int xioctl(int fd, int request, void* arg)
{
    for (int i = 0; i < 100; i++) {
        int r = ioctl(fd, request, arg);
        if (r != -1 || errno != EINTR) return r;
    }
    return -1;
}

int minmax(int min, int v, int max)
{
    return (v < min) ? min : (max < v) ? max : v;
}

void CameraDev::yuyv2rgb()
{
    uint8_t* yuyv = mCamera->head.start;

    for (size_t i = 0; i < mCamera->height; i++) {
        for (size_t j = 0; j < mCamera->width; j += 2) {
            size_t index = i * mCamera->width + j;
            int y0 = yuyv[index * 2 + 0] << 8;
            int u = yuyv[index * 2 + 1] - 128;
            int y1 = yuyv[index * 2 + 2] << 8;
            int v = yuyv[index * 2 + 3] - 128;
            mRgb[index * 3 + 0] = minmax(0, (y0 + 359 * v) >> 8, 255);
            mRgb[index * 3 + 1] = minmax(0, (y0 + 88 * v - 183 * u) >> 8, 255);
            mRgb[index * 3 + 2] = minmax(0, (y0 + 454 * u) >> 8, 255);
            mRgb[index * 3 + 3] = minmax(0, (y1 + 359 * v) >> 8, 255);
            mRgb[index * 3 + 4] = minmax(0, (y1 + 88 * v - 183 * u) >> 8, 255);
            mRgb[index * 3 + 5] = minmax(0, (y1 + 454 * u) >> 8, 255);
        }
    }
}

uint8_t *CameraDev::data()
{
    return mRgb;
}

uint8_t *CameraDev::raw()
{
    return mCamera->head.start;
}

CameraDev::Status CameraDev::status() const
{
    return mStatus;
}

void CameraDev::setStatus(const CameraDev::Status &status)
{
    mStatus = status;
}

CameraDev::CameraDev(char *device, uint16_t width, uint16_t height)
{
    bool res = false;

    res = cameraOpen(device, width, height);
    if(res) res = cameraInit();
    if(res) res = cameraStart();

    mRgb = new uint8_t[width * height * 3];

    mStatus = res ? Status::connected : Status::disconected;

    qDebug() << "USB CAMERA" << (res ? "OK" : "FAIL");
}

CameraDev::~CameraDev()
{
    delete mRgb;

    cameraStop();
    cameraFinish();
    cameraClose();
}

bool CameraDev::cameraOpen(const char * device, uint32_t width, uint32_t height)
{
    int fd = open(device, O_RDWR | O_NONBLOCK, 0);

    if (fd == -1)
        return false; //quit("open");

    mCamera = new camera_t;
    mCamera->fd = fd;
    mCamera->width = width;
    mCamera->height = height;
    mCamera->buffer_count = 0;
    mCamera->buffers = NULL;
    mCamera->head.length = 0;
    mCamera->head.start = NULL;

    return true;
    //  return camera;
}

bool CameraDev::cameraInit()
{

    struct v4l2_capability cap;

    if (xioctl(mCamera->fd, VIDIOC_QUERYCAP, &cap) == -1) return false; //quit("VIDIOC_QUERYCAP");
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) return false; //quit("no capture");
    if (!(cap.capabilities & V4L2_CAP_STREAMING)) return false;// quit("no streaming");

//    struct v4l2_cropcap cropcap;
//    memset(&cropcap, 0, sizeof cropcap);
//    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//    if (xioctl(camera->fd, VIDIOC_CROPCAP, &cropcap) == 0) {
//        struct v4l2_crop crop;
//        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//        crop.c = cropcap.defrect;
//        if (xioctl(camera->fd, VIDIOC_S_CROP, &crop) == -1) {
//            // cropping not supported
//        }
//    }

    struct v4l2_format format;
    memset(&format, 0, sizeof format);
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = mCamera->width;
    format.fmt.pix.height = mCamera->height;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    format.fmt.pix.field = V4L2_FIELD_NONE;
    if (xioctl(mCamera->fd, VIDIOC_S_FMT, &format) == -1) return false;// quit("VIDIOC_S_FMT");

    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof req);
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (xioctl(mCamera->fd, VIDIOC_REQBUFS, &req) == -1) return false;//quit("VIDIOC_REQBUFS");

    mCamera->buffer_count = req.count;
    mCamera->buffers = (buffer_t*)calloc(req.count, sizeof (buffer_t));

    size_t buf_max = 0;
    for (size_t i = 0; i < mCamera->buffer_count; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (xioctl(mCamera->fd, VIDIOC_QUERYBUF, &buf) == -1) return false;//      quit("VIDIOC_QUERYBUF");
        if (buf.length > buf_max) buf_max = buf.length;
        mCamera->buffers[i].length = buf.length;
        mCamera->buffers[i].start = (uint8_t*) mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED,
                                                   mCamera->fd, buf.m.offset);
        if (mCamera->buffers[i].start == MAP_FAILED) return false;//quit("mmap");
    }
    mCamera->head.start = (uint8_t*)malloc(buf_max);

    return true;
}


bool CameraDev::cameraStart()
{
    for (size_t i = 0; i < mCamera->buffer_count; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (xioctl(mCamera->fd, VIDIOC_QBUF, &buf) == -1) return false;//quit("VIDIOC_QBUF");
    }

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (xioctl(mCamera->fd, VIDIOC_STREAMON, &type) == -1) return false;// quit("VIDIOC_STREAMON");

    return true;
}

bool CameraDev::cameraStop()
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (xioctl(mCamera->fd, VIDIOC_STREAMOFF, &type) == -1) return false;// quit("VIDIOC_STREAMOFF");

    return true;
}

void CameraDev::cameraFinish()
{
    for (size_t i = 0; i < mCamera->buffer_count; i++) {
        munmap(mCamera->buffers[i].start, mCamera->buffers[i].length);
    }
    free(mCamera->buffers);
    mCamera->buffer_count = 0;
    mCamera->buffers = NULL;
    free(mCamera->head.start);
    mCamera->head.length = 0;
    mCamera->head.start = NULL;
}

bool CameraDev::cameraClose()
{
    if (close(mCamera->fd) == -1) return false;//quit("close");
    free(mCamera);

    return true;
}

void CameraDev::cameraInfo()
{
    struct v4l2_capability device_params;
    memset(&device_params, 0, sizeof device_params);
    if (xioctl(mCamera->fd, VIDIOC_QUERYCAP, &device_params) == -1) return;

    qDebug() << "driver: " << (char*)device_params.driver;
    qDebug() << "card: " << (char*)device_params.card;

//    struct v4l2_format format;
//    memset(&format, 0, sizeof format);
//    if (xioctl(camera->fd, VIDIOC_G_FMT, &format) == -1) return;

//    qDebug() << format.fmt.pix.width;
//    qDebug() << format.fmt.pix.height;
//    qDebug() << format.fmt.pix.pixelformat;
}

bool CameraDev::cameraCapture()
{
    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if (xioctl(mCamera->fd, VIDIOC_DQBUF, &buf) == -1) return false;
    memcpy(mCamera->head.start, mCamera->buffers[buf.index].start, buf.bytesused);
    mCamera->head.length = buf.bytesused;
    if (xioctl(mCamera->fd, VIDIOC_QBUF, &buf) == -1) return false;
    return true;
}

bool CameraDev::cameraFrame(struct timeval timeout) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(mCamera->fd, &fds);
    int r = select(mCamera->fd + 1, &fds, 0, 0, &timeout);
    if (r == -1) return false;//quit("select");
    if (r == 0) return false;
    return cameraCapture();
}
