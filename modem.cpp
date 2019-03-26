#include "modem.h"

#include <QDebug>

Modem::Modem(QString device, QObject *parent) :
    UartDevice(device, parent)
{
//    init(device);

    if(mStatus == Status::connected){
        quint8 attempt = 10;
        bool ok = false;

//        qDebug() << sendAnswer("AT&F").simplified();
        qDebug() << sendAnswer("ATZ").simplified();
        qDebug() << sendAnswer("ATE0").simplified();
        qDebug() << sendAnswer("ATI").simplified();

        qDebug() << sendAnswer("AT+CLIP=1").simplified();

        //        while (!(ok = sendAnswerCheck("AT", 1, "OK")) && attempt) {
//            attempt --;
//        }

//        if(ok){
//            send("ATE0");
//            qDebug() << ">>>" << answer().trimmed();
//        }
//        else
//            qDebug() << "err";
    }
    else
        qDebug() << "MODEM ERROR";
}

void Modem::send(QString cmd)
{
    qDebug() << "---" << cmd;
    cmd.append("\r\n");
    mSerial.write(cmd.toLocal8Bit());
    mSerial.flush();
}

QString Modem::sendAnswer(QString cmd)
{
    send(cmd);
    usleep(100000);
    return answer();
}

bool Modem::sendAnswerCheck(QString cmd, quint8 n, QString check)
{
    QString str = sendAnswer(cmd);
    qDebug() << str << str.section("\r\n", n, n);

    if(str.section("\r\n", 1, 1) == "ERROR")
        return false;

    return str.section("\r\n", n, n) == check;
}

QByteArray Modem::answer(int msec)
{
    mSerial.waitForReadyRead(msec);
    return mSerial.readAll();
}

bool Modem::sendUSSD(QString msg, QString &result)
{
    if(!sendAnswerCheck("AT+CMGF=1"))
        return false;

    if(!sendAnswerCheck(QString("AT+CUSD=1,\"%1\", 5").arg(msg)))
        return false;

    result = answer();

    return true;
}

bool Modem::restart() {
    if(!sendAnswerCheck("AT+CFUN=0"))
        return false;

    if(!sendAnswerCheck("AT+CFUN=1"))
        return false;
}

QString Modem::imei()
{
    QString str = sendAnswer("AT+GSN");

    if(str.section("\r\n", 3, 3) == "OK")
        return str.section("\r\n", 1, 1);
    else
        return "";
}

bool Modem::callAnswer()
{
    return sendAnswerCheck("A");
}

bool Modem::callNumber(QString phone)
{
    return sendAnswerCheck(QString("ATD%1").arg(phone));
}

bool Modem::sendSMS(QString phone, QString text)
{
    qDebug() << sendAnswer("AT+CMGF=1").simplified();
    if(sendAnswer(QString("AT+CMGS=\"%1\"").arg(phone)).trimmed() == ">"){
        mSerial.write(text.toLocal8Bit());
        mSerial.write(QByteArray(1,0x1A));
        qDebug() << answer();
    }
}

bool Modem::fmRadio(bool power, quint8 a)
{
    if(!power)
        if(!sendAnswerCheck("AT+FMCLOSE"))
            return false;
    if(a>1)
        return false;

    return sendAnswerCheck(QString("AT+FMOPEN=%1").arg(a));
}

bool Modem::fmRadioTune(quint16 freq)
{
    if(freq < 870 || freq > 1090)
        return false;

    return sendAnswerCheck(QString("AT+FMFREQ=%1").arg(freq));
}

bool Modem::fmRadioVolume(quint8 vol)
{
    if(vol > 6)
        return false;

    return sendAnswerCheck(QString("AT+FMVOLUME=%1").arg(vol));
}


