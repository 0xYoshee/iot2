#include "pininaction.h"

PinInAction::PinInAction(int pin, QObject *parent) :
    QThread(parent)
{
    mPin = pin;

    pinMode(mPin, INPUT);
    mState = digitalRead(mPin);

    if (!isRunning())
        start();
}

void PinInAction::run()
{
    while (1) {
        quint8 state = digitalRead(mPin);
        if(mState != state){
            mState = state;
            emit changeState(mState);
        }
    }
}

quint8 PinInAction::state() const
{
    return mState;
}
