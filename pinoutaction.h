#ifndef PINOUTACTION_H
#define PINOUTACTION_H

#include <QObject>
#include <QTimer>
#include <wiringPi.h>

class PinOutAction : public QObject
{
    Q_OBJECT
public:
    explicit PinOutAction(QString name, int pin, bool state = false, QObject *parent = nullptr);

    bool state() const;
    void setState(bool state);

    bool inverse();

    QString name() const;
    void setName(const QString &name);

    void start(quint16 secs);
    void stop();

    void pulse(quint32 delay);

signals:

public slots:
    void timeout();

private:
    int mPin;
    bool mState;

    QString mName;
    QTimer *mTimer;
};

#endif // PINOUTACTION_H
