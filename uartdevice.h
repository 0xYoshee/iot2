#ifndef UARTDEVICE_H
#define UARTDEVICE_H

#include <QObject>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class UartDevice : public QObject
{
    Q_OBJECT
public:
    enum Status{
        connected =1,
        disconected
    };

    explicit UartDevice(QString device, QObject *parent = nullptr);
    void init(const QString &device);

    UartDevice::Status status() const;

protected:
    QSerialPort mSerial;
    UartDevice::Status mStatus;
};

#endif // UARTDEVICE_H
