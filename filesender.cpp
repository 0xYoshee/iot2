#include "filesender.h"

FileSender::FileSender(QObject *parent)
    : QObject(parent),
      mqtt(false),
      telegram(false),
      dropbox(false)
{
//    mTimer = new QTimer();
//    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
//    mTimer->start(100);
}

void FileSender::setTelegram(Bot *telegramClient)
{
    telegram = true;
    mTelegram = telegramClient;
}

void FileSender::setMqtt(QMQTT::Client *mqttClient)
{
    mqtt = true;
    mMqtt = mqttClient;
}

void FileSender::setDropbox(QDropbox2 *dropboxClient)
{
    dropbox = true;
    mDropbox = dropboxClient;
}

void FileSender::send(QString name, QByteArray data, quint8 type)
{
    Image imageData;
    imageData.type = type;
    imageData.name = name;
    imageData.data = data;

    mImages.append(imageData);

//    mTimer->start(100);
}

void FileSender::send(QString name, QImage image, quint8 type)
{
    send(name, QImageToByte(image), type);
}

QByteArray FileSender::QImageToByte(QImage image)
{
    QByteArray arr;
    QBuffer buffer(&arr);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");
    return arr;
}

void FileSender::timeout()
{
    if(mImages.size()){
        Image imageData = mImages.at(0);

        if(imageData.type & Types::MQTT && mqtt){
            mMqtt->publish(QMQTT::Message(0, imageData.name, imageData.data, 2));
        }
        if(imageData.type & Types::TELEGRAM && telegram){
            QString lastImage = QString("/tmp/%1.jpg").arg(imageData.name);
            QFile file(lastImage);
            file.open(QIODevice::WriteOnly);
            file.write(imageData.data);
            file.close();
//            mTelegram->sendPhoto(BOT_CHAT, &file);
        }
        if(imageData.type & Types::DROPBOX && dropbox){

//            QFile local_file("/etc/fstab");
//            local_file.open(QIODevice::ReadOnly);
//            QByteArray data = local_file.readAll();
//            local_file.close();

//            QString db_path = QString("%1/%2").arg("/Alarm").arg("fstab");

//            QDropbox2File db_file(db_path, mDropbox);
//            db_file.setOverwrite();
//            db_file.open(QIODevice::WriteOnly | QIODevice::Truncate); //, true);
//            db_file.write(data);
//            db_file.flush();//, true);    // send buffered data to Dropbox
//            db_file.close();

            QString fname = QString("/Alarm/%1.jpg").arg(QDateTime::currentSecsSinceEpoch());
            QDropbox2File db_file(fname, mDropbox);
            db_file.setOverwrite();
            db_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
            db_file.write(imageData.data);
            db_file.flush();
            db_file.close();
        }
        mImages.remove(0);
    }
}
