#include "body.h"

#include <QDebug>
#include <QNetworkProxy>

Body::Body(QObject *parent) :
    QObject(parent),
    signalIndex(0),
    enableDisplay(false)
{
    sendPhotoTo = "D";

    wiringPiSetup();

    mCommands[CAMERA] = Commands::Camera;
    mCommands[RELAY1] = Commands::Relay1;
    mCommands[RELAY2] = Commands::Relay2;
    mCommands[RELAY3] = Commands::Relay3;
    mCommands[RELAY4] = Commands::Relay4;
    mCommands[RELAY5] = Commands::Relay5;

    mRelays[RELAY1] = new PinOutAction(RELAY1, PIN_RELAY1);
    mRelays[RELAY2] = new PinOutAction(RELAY2, PIN_RELAY2);
    mRelays[RELAY3] = new PinOutAction(RELAY3, PIN_RELAY3);
    mRelays[RELAY4] = new PinOutAction(RELAY4, PIN_RELAY4);
    mRelays[RELAY5] = new PinOutAction(RELAY5, PIN_RELAY5);

//    mModem = new Modem("/dev/modem0");

//    mDhtt = new DHTTheread(PIN_DHT);

    mBME280 = new BME280(DEV_TEMP);

    mCameraThread = new CameraThread(DEV_CAM1, 320, 240);
    enableCamera =(mCameraThread->status() == CameraDev::connected);

    mFileSender = new FileSender();
    mFileSender->moveToThread(new QThread(this));
    mFileSender->thread()->start();

    mSenderTimer = new QTimer();
    connect(mSenderTimer, SIGNAL(timeout()), this, SLOT(senderTimeout()));
    mSenderTimer->start(100);

    enableProxy = Settings::value("proxy/enable", false).toBool();

    if(enableProxy){
        QString host = Settings::value("proxy/host", "").toString();
        quint16 port = Settings::value("proxy/port", "").toInt();

        QString user = Settings::value("proxy/user", "").toString();
        QString pass = Settings::value("proxy/pass", "").toString();

        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName(host);
        proxy.setPort(port);
        proxy.setUser(user);
        proxy.setPassword(pass);
        QNetworkProxy::setApplicationProxy(proxy);
    }

    enableMQTT = Settings::value("mqtt/enable", false).toBool();

    if(enableMQTT){
        QString host = Settings::value("mqtt/host", "").toString();
        quint16 port = Settings::value("mqtt/port", "").toInt();

        QString cid = Settings::value("mqtt/cid", "").toString();
        QString cuser = Settings::value("mqtt/user", "").toString();
        QString cpass = Settings::value("mqtt/pass", "").toString();

        mqtt = new QMQTT::Client(host, port, false, false);
        mqtt->setClientId(cid);
        mqtt->setUsername(cuser);
        mqtt->setPassword(cpass.toUtf8());
        mqtt->setAutoReconnect(true);
        mqtt->connectToHost();

        mFileSender->setMqtt(mqtt);

        connect(mqtt,SIGNAL(received(QMQTT::Message)),this,SLOT(mqttReceived(QMQTT::Message)));
        connect(mqtt, SIGNAL(connected()), this, SLOT(mqttConnected()));
    }

    enableTelegram = Settings::value("telegram/enable", false).toBool();

    if(enableTelegram){
        QString botToken = Settings::value("telegram/token", "").toString();
        telegram = new Telegram::Bot(botToken, true, 500, 4);        
        connect(telegram, SIGNAL(message(Message)),this,SLOT(telegramReceived(Message)));
        mFileSender->setTelegram(telegram);
    }

    enableDropbox = Settings::value("dropbox/enable", false).toBool();
    if(enableDropbox){
        QString token = Settings::value("dropbox/token", "").toString();
        dropbox = new QDropbox2(token, this);
        connect(dropbox, SIGNAL(signal_errorOccurred(int, const QString&)), this, SLOT(signal_errorOccurred(int, const QString&)));
        mFileSender->setDropbox(dropbox);

        QDropbox2User info;
        dropbox->userInfo(info);

        QTextStream out(stdout);
        out << info.displayName() << ":\n";
        out << "\t            id: " << info.id() << "\n";
        out << "\t          type: " << info.type() << "\n";
        out << "\t          name: " << info.displayName() << "\n";
        out << "\t         email: " << info.email() << "\n";
        out << "\t emailVerified: " << (info.emailVerified() ? "true" : "false") << "\n";
        out << "\t    isDisabled: " << (info.isDisabled() ? "true" : "false") << "\n";
        out << "\t        locale: " << info.locale() << "\n";
        out << "\t  referralLink: " << info.referralLink().toString() << "\n";
        out << "\t      isPaired: " << (info.isPaired() ? "true" : "false") << "\n";
        out << "\t       country: " << info.country() << "\n";

    }

    enableMotion = Settings::value("motion/enable", false).toBool();
    if(enableCamera && enableMotion) {

        motionDetector.moveToThread(new QThread(this));
        motionDetector.thread()->start();
        connect(&motionDetector, SIGNAL(motion(QImage)), this, SLOT(motion(QImage)) );

        mMotionTimer = new QTimer();
        connect(mMotionTimer,SIGNAL(timeout()),this,SLOT(motionTimeOut()));
        mMotionTimer->start(10);

        webStream = new WebStream(mCameraThread);
        webStream->moveToThread(new QThread(this));
        webStream->thread()->start();

//        mMotionDetector = new MotionDetector();
//        mMotionDetector->set_threshold(30);
//        connect(mMotionDetector, SIGNAL(motion()), this, SLOT(motion()));
//        connect(&captureThread_, SIGNAL(output(const IplImage&)), &motionDetector_, SLOT(input(const IplImage&)));
//        connect(&motionDetector, SIGNAL(output(IplImage&)), this, SLOT(updateMotion(IplImage&)));

//        quint16 mTime = Settings::value("motion/time", 1000).toInt();
//        mMotionTimer = new QTimer();
//        connect(mMotionTimer,SIGNAL(timeout()),this,SLOT(motionTimeOut()));
//        mMotionTimer->start(mTime);
    }
    else
        enableMotion = false;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(1000);
}

Body::~Body()
{
    if(enableDisplay){
        ssd1306ClearScreen(LAYER0 | LAYER1) ;
        ssd1306DrawXBM(50, 15, start_width, start_height, start_bits, WHITE);
        ssd1306Refresh();
    }
}

void Body::changeRelayState(QString name, quint16 state)
{
    if(state<30){
        mRelays[name]->stop();
        mRelays[name]->setState(state);
//        relay(mRelays[name]);
    }
    else{
        mRelays[name]->start(state*1000*60);
//        if(name == RELAY1)
//            ui->sbRelay1->setInitState();
//        else if(name == RELAY2)
//            ui->sbRelay2->setInitState();
    }
}

void Body::timeout()
{
    if(mBME280->status() == BME280::connected){
        mBME280->update();
    }

    if(enableMQTT){
        mqtt->publish(QMQTT::Message(0, "tempState", QString("%1").arg(mBME280->cTemp()).toUtf8()));
        mqtt->publish(QMQTT::Message(0, "humiState", QString("%1").arg(mBME280->humidity()).toUtf8()));

//        mqtt->publish(QMQTT::Message(0, "tempState", QString("%1").arg(mDhtt->dht()->getTemp()).toUtf8()));
//        mqtt->publish(QMQTT::Message(0, "humiState", QString("%1").arg(mDhtt->dht()->getHum()).toUtf8()));
    }

    if(enableDisplay){
        ssd1306ClearScreen(LAYER0 | LAYER1) ;

        _font = (FONT_INFO*)&ubuntuMono_24ptFontInfo;
        ssd1306DrawString(15,  18, (int8_t*)QDateTime::currentDateTime().toString("HH:mm").toLatin1().data(), 1, WHITE, LAYER0);

        switch (signalIndex++) {
        case 10:
        case 0:
            ssd1306DrawCircle(6, 8, 0,  WHITE, LAYER0);
            if (signalIndex >= 10) signalIndex = 1;
            break;
        case 9:
        case 1:
            ssd1306DrawCircle(6, 8, 1,  WHITE, LAYER0);
            break;
        case 8:
        case 2:
            ssd1306DrawCircle(6, 8, 0,  WHITE, LAYER0);
            ssd1306DrawCircle(6, 8, 2,  WHITE, LAYER0);
            break;
        case 7:
        case 3:
            ssd1306DrawCircle(6, 8, 1,  WHITE, LAYER0);
            ssd1306DrawCircle(6, 8, 3,  WHITE, LAYER0);
            break;
        case 6:
        case 4:
            ssd1306DrawCircle(6, 8, 0,  WHITE, LAYER0);
            ssd1306DrawCircle(6, 8, 2,  WHITE, LAYER0);
            ssd1306DrawCircle(6, 8, 4,  WHITE, LAYER0);
            break;
        case 5:
            ssd1306DrawCircle(6, 8, 1,  WHITE, LAYER0);
            ssd1306DrawCircle(6, 8, 3,  WHITE, LAYER0);
            ssd1306DrawCircle(6, 8, 5,  WHITE, LAYER0);
            break;
        }

        for(quint8 index=1;index<=5;index++){
            QString rName = QString("relay%1").arg(index);
            if(mRelays[rName]->state())
                ssd1306DrawXBM(110, 10+(12*(index-1)), swon_width, swon_height, swon_bits, WHITE);
            else
                ssd1306DrawXBM(110, 6+(12*(index-1)), swon_width, swon_height+4, swoff_bits, WHITE);
        }

        _font = (FONT_INFO*)&ubuntuMono_8ptFontInfo;

        if(mBME280->cTemp() !=0 && mBME280->humidity() !=0){
            ssd1306DrawXBM( 2, 52, temper_width, temper_height, temper_bits, WHITE);
            ssd1306DrawString(14,  54, (int8_t*)QString("%1°C").arg(mBME280->cTemp(), 0, 'f', 2).toLatin1().data(), 1, WHITE, LAYER0);

            ssd1306DrawXBM( 54, 52, humidity_width, humidity_height, humidity_bits, WHITE);
            ssd1306DrawString(68,  54, (int8_t*)QString("%1%").arg(mBME280->humidity(), 0,'f', 2).toLatin1().data(), 1, WHITE, LAYER0);
        }

//        if(mDhtt->dht()->getTemp() !=0 && mDhtt->dht()->getHum()!=0){
//            ssd1306DrawXBM( 2, 52, temper_width, temper_height, temper_bits, WHITE);
//            ssd1306DrawString(14,  54, (int8_t*)QString("%1°C").arg(mDhtt->dht()->getTemp()).toLatin1().data(), 1, WHITE, LAYER0);

//            ssd1306DrawXBM( 54, 52, humidity_width, humidity_height, humidity_bits, WHITE);
//            ssd1306DrawString(68,  54, (int8_t*)QString("%1%").arg(mDhtt->dht()->getHum()).toLatin1().data(), 1, WHITE, LAYER0);
//        }

        if(enableTelegram)
            ssd1306DrawString(15,  4, (int8_t*)QString("T").toLatin1().data(), 1, WHITE, LAYER0);
        if(enableMQTT)
            ssd1306DrawString(22,  4, (int8_t*)QString("Q").toLatin1().data(), 1, WHITE, LAYER0);
        if(enableCamera)
            ssd1306DrawString(28,  4, (int8_t*)QString("C").toLatin1().data(), 1, WHITE, LAYER0);
        if(enableMotion){
            ssd1306DrawString(34,  4, (int8_t*)QString("M").toLatin1().data(), 1, WHITE, LAYER0);
            ssd1306DrawString(50,  4, (int8_t*)(QString("[%1]").arg(sendPhotoTo.toUpper())).toLatin1().data(), 1, WHITE, LAYER0);
        }

        ssd1306Refresh();
    }
}

void Body::senderTimeout()
{
    mFileSender->timeout();
}

void Body::motionTimeOut()
{
//    QImage img = *mCameraThread->image();
//    IplImage *iimg = QImage2IplImage(&img);
//    mMotionDetector->input(iimg);

    motionDetector.input2(mCameraThread->image());
}

void Body::motion(QImage image)
{
//    if(mAlarmState.dropbox){
//        mFileSender->send("imageMotion", image, Types::ALL);
//    }
//    else{
        quint8 type =(Types::MQTT | Types::DROPBOX);
        mFileSender->send("imageMotion", image, type );
//    }
}

void Body::mqttReceived(QMQTT::Message message)
{
    qDebug() << message.topic() << message.payload();

    QString topic = message.topic();
    QString param = message.payload();

    execCommand(topic, param);
}

void Body::mqttConnected()
{
    for(QString key : mCommands.keys()){
        mqtt->subscribe(key);
    }

    qDebug() << "MQTT OK";
}

void Body::telegramReceived(Message m)
{
    if (telegram && m.type == Message::TextType) {
        if(m.string.toLower().contains("=")){
            QString cmd = m.string.toLower().section('=',0,0);
            QString value = m.string.toLower().section('=',1,1);

            if(cmd == "relay"){
                for(quint8 i=1;i<=5;i++){
                    QString r = QString("%1%2").arg(cmd).arg(i);
                    execCommand(r, value);
                    telegram->sendMessage(m.chat.id, QString("relay%1=%2").arg(i).arg(mRelays[r]->state()) );
                }
            }
            else if(cmd.contains("relay")){
                if(mRelays.contains(cmd)){
                    execCommand(cmd, value);
                    telegram->sendMessage(m.chat.id, QString("%1=%2").arg(cmd).arg(mRelays[cmd]->state()) );
                }
                else{
                    telegram->sendMessage(m.chat.id, m.string+" ???");
                }
            }
            if(cmd == "send"){
                sendPhotoTo = value;
                telegram->sendMessage(m.chat.id, QString("sendPhotoTo=%1").arg(sendPhotoTo) );
            }
            else{
                telegram->sendMessage(m.chat.id, m.string+" ???");
            }
        }
        else if(mRelays.contains(m.string.toLower())){
            telegram->sendMessage(m.chat.id, QString("%1=%2").arg(m.string.toLower()).arg(mRelays[m.string.toLower()]->state()) );
        }
        else if(m.string.toLower() == "reboot") {
            if(enableDisplay){
                timer->stop();
                ssd1306ClearScreen(LAYER0 | LAYER1) ;
                ssd1306DrawXBM(50, 15, start_width, start_height, start_bits, WHITE);
                ssd1306Refresh();
            }
           QProcess::startDetached("reboot");
        }
        else if(m.string.toLower() == "send"){
            telegram->sendMessage(m.chat.id, QString("sendPhotoTo=%1").arg(sendPhotoTo) );
        }
        else if(m.string.toLower() == "temp") {
            telegram->sendMessage(m.chat.id, QString("t= %1 °C").arg(mBME280->cTemp()));
            telegram->sendMessage(m.chat.id, QString("h= %1 RH").arg(mBME280->humidity()));
            telegram->sendMessage(m.chat.id, QString("p= %1 mm Hg").arg(mBME280->pressure()/1.33322));

//            telegram->sendMessage(m.chat.id, QString("%1°C %2%").arg(mDhtt->dht()->getTemp()).arg(mDhtt->dht()->getHum()));
        }       
        else if(m.string.toLower().contains("cam")){
            if(enableCamera) {
                QString lastImage = QString("/tmp/%1.png").arg("cam");
                mCameraThread->image()->save(lastImage,"PNG");                
                QFile file(lastImage);
                telegram->sendPhoto(m.chat.id, &file);
            }
            else
                telegram->sendMessage(m.chat.id, "no cam");
        }
        else{
            telegram->sendMessage(m.chat.id, m.string+" ???");
        }
    }
}

QString Body::execCommand(QString topic, QString param)
{
    if (!mCommands.contains(topic))
      return "";

    quint8 cid = mCommands[topic];
    topic = topic + "State";

    QString rName;

    switch (cid) {
    case Commands::Relay1:
    case Commands::Relay2:
    case Commands::Relay3:
    case Commands::Relay4:
    case Commands::Relay5:
        rName = QString("relay%1").arg((quint8)cid);
        if (param == "on" || param == "1") {
            changeRelayState(rName, 1);
        }
        else if (param == "2") {
            changeRelayState(rName, mRelays[rName]->inverse());
        }
        else if (param == "off" || param == "0") {
            changeRelayState(rName, 0);
        }
        else {
            quint16 rtime = param.toInt();
            if (rtime >= 30)
                changeRelayState(rName, rtime);
        }
    break;

    case Commands::Camera:
        if(enableCamera) {
            QByteArray arr;
            QBuffer buffer(&arr);
            buffer.open(QIODevice::WriteOnly);
            mCameraThread->image()->save(&buffer, "JPG");
            mqtt->publish(QMQTT::Message(0, "image1", arr, 2));
            return "";
        }

    default:
        break;
    }

    if(enableMQTT)
        mqtt->publish(QMQTT::Message(0, topic, param.toUtf8(), 1, false));

    return param;
}

void Body::signal_errorOccurred(int errorcode, const QString &errormessage)
{
    qDebug() << errormessage;
}

void Body::setDisplay(Display *display)
{
    mDisplay = display;
    enableDisplay = true;
}
