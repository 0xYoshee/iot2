#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QtCore>
#include <QImage>

#include "cameradev.h"

class CameraThread : public QThread
{
    Q_OBJECT
public:
    explicit CameraThread(QString device, quint16 width=640, quint16 height=480, QObject *parent = 0);
    void run();

    uint8_t *data();

    QImage *image();

    QByteArray imageData();

    CameraDev::Status status() const;

private:
    CameraDev *mCameraDev;
    QImage *mImage;

    quint16 mWidth;
    quint16 mHeight;
};

#endif // CAMERATHREAD_H
