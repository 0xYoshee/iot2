#ifndef WEBSTREAM_H
#define WEBSTREAM_H

#include <QObject>

#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>

#include "camerathread.h"

class WebStream : public QObject
{
    Q_OBJECT
public:
    explicit WebStream(CameraThread *camera, quint32 port=80, QObject *parent = nullptr);
    ~WebStream();

    void sendContent(QTcpSocket *socket);//, QByteArray data);
signals:

public slots:
    void newConnection();
    void readyRead();
    void socketDisconnected();
private:
    QTcpServer *server;

    QVector <QTcpSocket *> sockets;

    CameraThread *mCameraThread;
};

#endif // WEBSTREAM_H
