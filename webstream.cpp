#include "webstream.h"

WebStream::WebStream(CameraThread *camera, quint32 port,QObject *parent) : QObject(parent)
{
    mCameraThread = camera;
    server  = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    server->listen(QHostAddress::Any, port);
}

WebStream::~WebStream()
{
    disconnect(server, SIGNAL(newConnection()));
    delete server;
}

void WebStream::newConnection()
{
    while (server->hasPendingConnections())
    {
        QTcpSocket *socket = server->nextPendingConnection();
        sockets.append(socket);
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

//        ui->teLog->insertPlainText(QString("подключился %1\n").arg(socket->peerAddress().toString()));
    }

}
/*
void WebStream::sendContent(QTcpSocket *socket, QByteArray data)
{
    QByteArray ContentType = ("HTTP/1.1 200 OK\r\n" \
                              "Content-Type: image/png\r\n" \
                              "Content-Length: ");

    ContentType.append(QString::number(data.length()));
    ContentType.append("\r\n\r\n");

    socket->write(ContentType);
    socket->write(data);
    socket->flush();
    socket->waitForBytesWritten(10000);

}
*/

void WebStream::sendContent(QTcpSocket *socket)//, QByteArray data){
{
//    for(quint32 index=0;index<sockets.size();index++){
//        QTcpSocket *socket = sockets.at(index);
//        if(!socket->isOpen()) break;
//        socket->readAll();

        QByteArray ContentType = ("HTTP/1.0 200 OK\r\n" \
                                  "Server: 127.0.0.1\r\n" \
                                  "Cache-Control: no-cache\r\n" \
                                  "Cache-Control: private\r\n" \
                                  "Content-Type: multipart/x-mixed-replace;boundary=img\r\n\r\n");

        socket->write(ContentType);

        QByteArray data = mCameraThread->imageData();

        QByteArray BoundaryString = ("--img\r\n" \
                                     "Content-Type: image/jpg\r\n" \
                                     "Content-Length: ");

        BoundaryString.append(QString::number(data.length()));
        BoundaryString.append("\r\n\r\n");

        socket->write(BoundaryString);
        socket->write(data);

        QByteArray EndString = ("--img\r\n");
        socket->write(EndString);

        socket->flush();
        socket->waitForBytesWritten(10000);
//    }
}


void WebStream::readyRead()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->readAll();
    sendContent(socket);//, mCameraThread->imageData());
    socket->close();
}

//void WebStream::readyRead()
//{
//    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());

//    socket->readAll();

////    QByteArray ContentType = ("HTTP/1.0 200 OK\r\n" \
////                              "Server: 127.0.0.1 example server\r\n" \
////                              "Cache-Control: no-cache\r\n" \
////                              "Cache-Control: private\r\n" \
////                              "Content-Type: multipart/x-mixed-replace;boundary=--boundary\r\n\r\n");

////    socket->write(ContentType);

////    QFile file("d:/111.jpg");
////    file.open(QIODevice::ReadOnly);

////    QDataStream in(&file);

////    QByteArray CurrentImg;//(QByteArray::fromStdString(content));
////    CurrentImg.resize(file.size());
////    in.readRawData(CurrentImg.data(),file.size());
////    file.close();

////    QByteArray BoundaryString = ("--boundary\r\n" \
////                                 "Content-Type: image/jpeg\r\n" \
////                                 "Content-Length: ");

////    BoundaryString.append(QString::number(CurrentImg.length()));
////    BoundaryString.append("\r\n\r\n");

////    socket->write(BoundaryString);
////    socket->write(CurrentImg); // Write The Encoded Image

//    socket->flush();
//    //    socket->close();
//}

void WebStream::socketDisconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    qint32 ind = sockets.indexOf(socket);

    if(socket->isOpen())
        socket->close();

    disconnect(socket, SIGNAL(readyRead()));
    disconnect(socket, SIGNAL(disconnected()));

    sockets.remove(ind);
}
