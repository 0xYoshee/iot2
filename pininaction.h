#ifndef PININACTION_H
#define PININACTION_H

#include <QThread>
#include <wiringPi.h>

class PinInAction : public QThread
{
    Q_OBJECT
public:
    explicit PinInAction(int pin, QObject *parent = nullptr);
    void run();

    quint8 state() const;

signals:
    void changeState(quint8 state);

private:
    quint8 mPin;
    quint8 mState;

};

#endif // PININACTION_H
