#include "uartdevice.h"

UartDevice::UartDevice(QString device, QObject *parent) :
    QObject(parent)
{
    init(device);
}

void UartDevice::init(const QString &device)
{
    mSerial.close();
//    m_serial.setReadBufferSize(1024);

    if(!mSerial.isOpen())
    {
        mSerial.setPortName(device);
        mStatus = Status::connected;

        if (!mSerial.open(QIODevice::ReadWrite)) {
//            processError(tr("Ошибка открытия порта %1")
//                         .arg(m_serial.portName()));
            mStatus = Status::disconected;
            return;
        }

        if (!mSerial.setBaudRate(QSerialPort::Baud115200)){
//            processError(tr("Ошибка установки скорости 9600 на порте %1")
//                         .arg(m_serial.portName()));
            mStatus = Status::disconected;
            return;
        }

        if (!mSerial.setDataBits(QSerialPort::Data8)) {
//            processError(tr("Ошибка установки 8 data bits на порте %1")
//                         .arg(m_serial.portName()));
            mStatus = Status::disconected;
            return;
        }

        if (!mSerial.setParity(QSerialPort::NoParity)) {
//            processError(tr("Ошибка установки no patity на порте %1")
//                         .arg(m_serial.portName()));
            mStatus = Status::disconected;
            return;
        }

        if (!mSerial.setStopBits(QSerialPort::OneStop)) {
//            processError(tr("Ошибка установки 1 stop bit на порте %1")
//                         .arg(m_serial.portName()));
            mStatus = Status::disconected;
            return;
        }

        if (!mSerial.setFlowControl(QSerialPort::NoFlowControl)) {
//            processError(tr("Ошибка установки no flow control на порте %1")
//                         .arg(m_serial.portName()));
            mStatus = Status::disconected;
            return;
        }
    }
}

UartDevice::Status UartDevice::status() const
{
    return mStatus;
}
