#include "dht.h"

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <dirent.h>
//#include <fcntl.h>
//#include <assert.h>
//#include <sys/mman.h>
//#include <sys/types.h>
//#include <sys/stat.h>

#include <unistd.h>

#include <wiringPi.h>

#include <QDebug>

DHT::DHT(int pin)
{
    mTemp = 0;
    mHum = 0;

    mDelta = 5;

    qDebug() << "DHT OK";
    mPin = pin;

    mType = DHT22;
    pinMode(mPin,INPUT);
}

DHT::DHT(int pin, int type)
{
    mTemp = 0;
    mHum = 0;

    mPin = pin;
    mType = type;
    pinMode(mPin,INPUT);
}

int DHT::update()
{
    int counter = 0;
    int laststate = 1;
    int j=0;
    int i=0;

    pinMode(mPin, OUTPUT);// gpio.setDirection(0);
    digitalWrite(mPin,HIGH);// gpio.write(1);
    usleep(500000);
    digitalWrite(mPin,LOW);//gpio.write(0);
    usleep(20000);
    pinMode(mPin,INPUT);//gpio.setDirection(1);

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    for (i=0; i< MAXTIMINGS; i++) {
        counter = 0;
        while ( digitalRead(mPin) == laststate) {
            counter++;
            DELAY(800);
            if (counter == 100)
                break;
        }
        laststate = digitalRead(mPin);
        if (counter == 100) break;
        bits[bitidx++] = counter;

        if ((i>3) && (i%2 == 0)) {
            data[j/8] <<= 1;
            if (counter > 16)
                data[j/8] |= 1;
            j++;
        }
    }

    if ((j >= 39) &&
            (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {

        if (mType == DHT11) {
            mTemp = data[2];
            mHum = data[0];
        }

        if (mType == DHT22) {
            float f, h;
            h = data[0] * 256 + data[1];
            h /= 10;

            f = (data[2] & 0x7F)* 256 + data[3];
            f /= 10.0;

            if (data[2] & 0x80)
                f *= -1;

//            if( mDelta < abs(temp-f) )
//                return 1;

            if(f!=0 && h!=0){
                mTemp = f;
                mHum = h;
            }
        }
        return 1;
    }
    return 0;
}

float DHT::getTemp() const
{
    return mTemp;
}

float DHT::getHum() const
{
    return mHum;
}

int DHT::getType() const
{
    return mType;
}

void DHT::setType(int type)
{
    mType = type;
}

DHTTheread::DHTTheread(int pin, QObject *parent):
    QThread(parent),
    mPin(pin)
{
    mDht = new DHT(pin);

    if (!isRunning())
        start();
}

void DHTTheread::run()
{
    while (1) {
        usleep(1000);

//        if(m_pause){
//            usleep(100000);
//        }

        mDht->update();
    }
}

DHT *DHTTheread::dht() const
{
    return mDht;
}
