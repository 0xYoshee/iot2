#include "pinoutaction.h"

#include <unistd.h>

#include <QDebug>

PinOutAction::PinOutAction(QString name, int pin, bool state, QObject *parent) :
    QObject(parent),
    mPin(pin),
    mName(name),
    mState(state)
{
    qDebug() << "RALAY" << mPin << "OK";

    pinMode(mPin, OUTPUT);
    digitalWrite(mPin, state ? HIGH : LOW);

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
}

bool PinOutAction::state() const
{
    return mState;
}

void PinOutAction::setState(bool state)
{
    mState = state;
    digitalWrite(mPin, state ? HIGH : LOW);
}

bool PinOutAction::inverse()
{
    setState(!mState);
    return mState;
}

QString PinOutAction::name() const
{
    return mName;
}

void PinOutAction::setName(const QString &name)
{
    mName = name;
}

void PinOutAction::start(quint16 secs)
{
    if(mTimer->isActive())
        mTimer->stop();

    setState(true);
    mTimer->start(secs);
}

void PinOutAction::stop()
{
    setState(false);
    mTimer->stop();
}

void PinOutAction::pulse(quint32 delay)
{
    setState(true);
    usleep(delay);
    setState(false);
}

void PinOutAction::timeout()
{
    inverse();
}
