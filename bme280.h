#ifndef BME280_H
#define BME280_H

class BME280
{
public:
    enum Status {
        connected =1,
        disconected
    };

    BME280(char *device);
    ~BME280();

    bool init(char *device);
    void update();

    BME280::Status status() const;

    float pressure() const;
    float cTemp() const;
    float fTemp() const;
    float humidity() const;

private:
    BME280::Status mStatus;

    int file;

    float mPressure;
    float mCTemp;
    float mFTemp;
    float mHumidity;

    int dig_T[3];
    int dig_P[9];
    int dig_H[6];
};

#endif // BME280_H
