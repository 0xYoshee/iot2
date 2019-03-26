#ifndef CAMERADEV_H
#define CAMERADEV_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <asm/types.h>
#include <linux/videodev2.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
  uint8_t* start;
  size_t length;
} buffer_t;

typedef struct {
  int fd;
  uint32_t width;
  uint32_t height;
  size_t buffer_count;
  buffer_t* buffers;
  buffer_t head;
} camera_t;

class CameraDev
{
public:

    enum Status {
        connected =1,
        disconected
    };

    CameraDev(char *device, uint16_t width, uint16_t height);
    ~CameraDev();

    bool cameraOpen(const char *device, uint32_t width, uint32_t height);

    bool cameraInit();
    bool cameraStart();

    bool cameraStop();
    void cameraFinish();
    bool cameraClose();

    void cameraInfo();

    bool cameraCapture();
    bool cameraFrame(timeval timeout);

    void yuyv2rgb();

    uint8_t *data();
    uint8_t *raw();

    CameraDev::Status status() const;
    void setStatus(const CameraDev::Status &status);

private:
    camera_t* mCamera;

    uint8_t *mRgb;

    CameraDev::Status mStatus;
};

#endif // CAMERADEV_H
