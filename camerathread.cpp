#include "camerathread.h"

CameraThread::CameraThread(QString device, quint16 width, quint16 height, QObject *parent) :
    QThread(parent),
    mWidth(width),
    mHeight(height)
{
    mCameraDev = new CameraDev(device.toLocal8Bit().data(), width, height);

    if(mCameraDev->status() == CameraDev::connected) {
        if (!isRunning())
            start();
    }
}

void CameraThread::run()
{
    struct timeval timeout;
    timeout.tv_sec=2;
    timeout.tv_usec=0;

    while(1) {
        if(!mCameraDev->cameraFrame(timeout))
            continue;
    }
}

uint8_t *CameraThread::data()
{
    return mCameraDev->data();
}

QImage *CameraThread::image()
{
    mCameraDev->yuyv2rgb();
    mImage = new QImage(mCameraDev->data(), mWidth, mHeight, QImage::Format_RGB888 );
    return mImage;
}

QByteArray CameraThread::imageData()
{
    QByteArray arr;
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    image()->save(&buffer, "JPG");
    return arr;
}

CameraDev::Status CameraThread::status() const
{
    return mCameraDev->status();
}

