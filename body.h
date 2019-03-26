#ifndef BODY_H
#define BODY_H

#include <QObject>
#include <QTimer>
#include <QDateTime>

#include "qmqtt.h"
#include "qttelegrambot.h"

#include "settings.h"

#include "pinoutaction.h"
#include "dht.h"
//#include "modem.h"

#include "cameradev.h"
#include "camerathread.h"

#include "images.h"
#include "defines.h"

#include "display.h"

#include "bme280.h"

#include "filesender.h"

#include "imageconverter.h"

#include "motiondetector.h"

#include "webstream.h"

//#include "opencv/motiondetector.h"

//#include "qdropbox2.h"
//#include "qdropbox2file.h"
//#include "qdropbox2folder.h"

using namespace Telegram;

extern FONT_INFO *_font;

enum Commands
{
    Camera=0,
    Relay1,
    Relay2,
    Relay3,
    Relay4,
    Relay5,
};

class Body : public QObject
{
    Q_OBJECT
public:
    explicit Body(QObject *parent = nullptr);
    ~Body();

    void changeRelayState(QString name, quint16 state);


    void setDisplay(Display *display);

signals:

public slots:
    void timeout();
//    void cameraTimeOut();
    void senderTimeout();
    void motionTimeOut();

    void motion(QImage image);
//    void updateMotion(IplImage &iimag);

    void mqttReceived(QMQTT::Message message);
    void mqttConnected();

    void telegramReceived(Message m);

    QString execCommand(QString topic, QString param);

    void signal_errorOccurred(int errorcode, const QString& errormessage);

private:
    QTimer *timer;
    quint8 signalIndex;

//    QTimer *mCameraTimer;
    CameraThread *mCameraThread;

//    Modem *mModem;
//    DHTTheread *mDhtt;
    BME280 *mBME280;

    QMQTT::Client *mqtt;
    Bot *telegram;

    QMap<QString, quint8> mCommands;

    QMap<QString, PinOutAction*> mRelays;

    Display *mDisplay;

//    MotionDetector *mMotionDetector;
    QTimer *mMotionTimer;
    QTimer *mSenderTimer;

    QDropbox2 *dropbox;

    QString sendPhotoTo;

    WebStream *webStream;

    FileSender *mFileSender;

    MotionDetector motionDetector;

    bool enableTelegram;
    bool enableMQTT;
    bool enableCamera;
    bool enableDisplay;
    bool enableProxy;
    bool enableMotion;
    bool enableDropbox;
};

#endif // BODY_H
