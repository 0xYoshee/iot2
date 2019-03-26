#ifndef FILESENDER_H
#define FILESENDER_H

#include <QObject>
#include <QImage>
#include <QBuffer>
//#include <QTimer>

#include "qdropbox2.h"
#include "qdropbox2file.h"
//#include "qdropbox2folder.h"

#include "qmqtt.h"
#include "qttelegrambot.h"

#include "defines.h"

using namespace Telegram;

enum Types
{
    NONE =0,
    MQTT =1,
    TELEGRAM=2,
    DROPBOX=4,
    ALL=7,
};

struct Image{
    quint8 type;
    QString name;
    QByteArray data;
};

class FileSender : public QObject
{
    Q_OBJECT
public:
    explicit FileSender(QObject *parent = nullptr);

    void setTelegram(Bot *telegramClient);
    void setMqtt(QMQTT::Client *mqttClient);
    void setDropbox(QDropbox2 *dropboxClient);

    void send(QString name, QByteArray data, quint8 type=Types::NONE);
    void send(QString name, QImage image, quint8 type=Types::NONE);

    static QByteArray QImageToByte(QImage image);

signals:

public slots:
    void timeout();

private:
    bool mqtt;
    bool telegram;
    bool dropbox;

    Bot *mTelegram;
    QMQTT::Client *mMqtt;
    QDropbox2 *mDropbox;

    QVector<Image> mImages;

//    QTimer *mTimer;
};

#endif // FILESENDER_H
