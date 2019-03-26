#ifndef DHT_H
#define DHT_H

#define MAXTIMINGS 100

#define DHT11  11
#define DHT22  22
#define AM2302 22

#define DELAY(sec) for(int i=0;i<sec;i++) asm("NOP");

#include <QtCore>

class DHT
{
public:
    DHT(int pin);
    DHT(int pin, int type);

    int update();

    float getTemp() const;
    float getHum() const;

    int getType() const;
    void setType(int type);

private:
    int mType;
    int mPin;

    int bits[250], data[100];
    int bitidx = 0;

    float mTemp, mHum;

    float mDelta;
};

class DHTTheread : public QThread
{
    Q_OBJECT
public:
    explicit DHTTheread(int pin, QObject *parent = 0);
    void run();

    int mPin;
    bool mPause;

    bool pause() const;
    void setPause(bool pause);

    DHT *dht() const;

signals:

private:
    DHT *mDht;
};


#endif // DHT_H
