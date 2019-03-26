#ifndef MODEM_H
#define MODEM_H

//#include <QObject>

//#include <QtSerialPort/QSerialPort>
//#include <QtSerialPort/QSerialPortInfo>

#include "uartdevice.h"

#include <unistd.h>

class Modem : public UartDevice
{
    Q_OBJECT
public:
//    enum Status{
//        connected =1,
//        disconected
//    };

    Modem(QString device, QObject *parent = 0);

//    void init(const QString &port);

    void send(QString cmd);

    QString sendAnswer(QString cmd);
    bool sendAnswerCheck(QString cmd, quint8 n=1, QString check="OK");

    QByteArray answer(int msec=3000);

    bool sendUSSD(QString msg, QString &result);

    bool restart();

    QString imei();

    bool callAnswer();
    bool callNumber(QString phone);

    bool sendSMS(QString phone, QString text);

    bool fmRadio(bool power, quint8 a=0);
    bool fmRadioTune(quint16 freq);
    bool fmRadioVolume(quint8 vol);

//private:
//    QSerialPort mSerial;
//    Status mStatus;
};

#endif // MODEM_H
