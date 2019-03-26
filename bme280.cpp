#include "bme280.h"

//#include <stdio.h>
//#include <stdlib.h>
//#include <stdint.h>
//#include <string.h>
//#include <errno.h>

//#include <sys/mman.h>

//#include <sys/time.h>
//#include <sys/types.h>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <unistd.h>

#include <QDebug>

BME280::BME280(char *device)
{
    mPressure = 0;
    mCTemp = 0;
    mFTemp = 0;
    mHumidity = 0;

    bool res = false;

    res = init(device);

    mStatus = res ? Status::connected : Status::disconected;

    qDebug() << "BME280" << (res ? "OK" : "FAIL");
}

BME280::~BME280()
{

}

bool BME280::init(char *device)
{
    file = open(device, O_RDWR | O_NONBLOCK, 0);

    if (file == -1)
        return false;

    ioctl(file, I2C_SLAVE, 0x76);

    // Read 24 bytes of data from register(0x88)
    char reg[1] = {0x88};
    write(file, reg, 1);

    char b1[24] = {0};
    if(read(file, b1, 24) != 24)
    {
        printf("Error : Input/Output error \n");
        return false;
    }

    // Convert the data
    // temp coefficents
    dig_T[0] = (b1[0] + b1[1] * 256);
    dig_T[1] = (b1[2] + b1[3] * 256);
    if(dig_T[1] > 32767)
        dig_T[1] -= 65536;

    dig_T[2] = (b1[4] + b1[5] * 256);
    if(dig_T[2] > 32767)
        dig_T[2] -= 65536;

    // pressure coefficents
    dig_P[0] = (b1[6] + b1[7] * 256);
    dig_P[1] = (b1[8] + b1[9] * 256);
    if(dig_P[1] > 32767)
        dig_P[1] -= 65536;

    dig_P[2] = (b1[10] + b1[11] * 256);
    if(dig_P[2] > 32767)
        dig_P[2] -= 65536;

    dig_P[3] = (b1[12] + b1[13] * 256);
    if(dig_P[3] > 32767)
        dig_P[3] -= 65536;

    dig_P[4] = (b1[14] + b1[15] * 256);
    if(dig_P[4] > 32767)
        dig_P[4] -= 65536;

    dig_P[5] = (b1[16] + b1[17] * 256);
    if(dig_P[5] > 32767)
        dig_P[5] -= 65536;

    dig_P[6] = (b1[18] + b1[19] * 256);
    if(dig_P[6] > 32767)
        dig_P[6] -= 65536;

    dig_P[7] = (b1[20] + b1[21] * 256);
    if(dig_P[7] > 32767)
        dig_P[7] -= 65536;

    dig_P[8] = (b1[22] + b1[23] * 256);
    if(dig_P[8] > 32767)
        dig_P[8] -= 65536;

    // Read 1 byte of data from register(0xA1)
    reg[0] = 0xA1;
    write(file, reg, 1);

    char data[8] = {0};
    read(file, data, 1);

    dig_H[0] = data[0];

    // Read 7 bytes of data from register(0xE1)
    reg[0] = 0xE1;
    write(file, reg, 1);
    read(file, b1, 7);

    // Convert the data
    // humidity coefficents
    dig_H[1] = (b1[0] + b1[1] * 256);
    if(dig_H[1] > 32767)
        dig_H[1] -= 65536;

    dig_H[2] = b1[2] & 0xFF ;
    dig_H[3] = (b1[3] * 16 + (b1[4] & 0xF));
    if(dig_H[3] > 32767)
        dig_H[3] -= 65536;

    dig_H[4] = (b1[4] / 16) + (b1[5] * 16);
    if(dig_H[4] > 32767)
        dig_H[4] -= 65536;

    dig_H[5] = b1[6];
    if(dig_H[5] > 127)
        dig_H[5] -= 256;

    // Select control humidity register(0xF2)
    // Humidity over sampling rate = 1(0x01)
    char config[2] = {0};
    config[0] = 0xF2;
    config[1] = 0x01;
    write(file, config, 2);
    // Select control measurement register(0xF4)
    // Normal mode, temp and pressure over sampling rate = 1(0x27)
    config[0] = 0xF4;
    config[1] = 0x27;
    write(file, config, 2);
    // Select config register(0xF5)
    // Stand_by time = 1000 ms(0xA0)
    config[0] = 0xF5;
    config[1] = 0xA0;
    write(file, config, 2);

    return true;
}

void BME280::update()
{
    char reg[1];
    char data[8];

    // Read 8 bytes of data from register(0xF7)
    // pressure msb1, pressure msb, pressure lsb, temp msb1, temp msb, temp lsb, humidity lsb, humidity msb
    reg[0] = 0xF7;
    write(file, reg, 1);
    read(file, data, 8);

    // Convert pressure and temperature data to 19-bits
    long adc_p = ((long)(data[0] * 65536 + ((long)(data[1] * 256) + (long)(data[2] & 0xF0)))) / 16;
    long adc_t = ((long)(data[3] * 65536 + ((long)(data[4] * 256) + (long)(data[5] & 0xF0)))) / 16;
    // Convert the humidity data
    long adc_h = (data[6] * 256 + data[7]);

    // Temperature offset calculations
    float var1 = (((float)adc_t) / 16384.0 - ((float)dig_T[0]) / 1024.0) * ((float)dig_T[1]);
    float var2 = ((((float)adc_t) / 131072.0 - ((float)dig_T[0]) / 8192.0) *
                  (((float)adc_t)/131072.0 - ((float)dig_T[0])/8192.0)) * ((float)dig_T[2]);
    float t_fine = (long)(var1 + var2);
    mCTemp = (var1 + var2) / 5120.0;
    mFTemp = mCTemp * 1.8 + 32;

    // Pressure offset calculations
    var1 = ((float)t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((float)dig_P[5]) / 32768.0;
    var2 = var2 + var1 * ((float)dig_P[4]) * 2.0;
    var2 = (var2 / 4.0) + (((float)dig_P[3]) * 65536.0);
    var1 = (((float) dig_P[2]) * var1 * var1 / 524288.0 + ((float) dig_P[1]) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((float)dig_P[0]);

    float p = 1048576.0 - (float)adc_p;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((float) dig_P[8]) * p * p / 2147483648.0;
    var2 = p * ((float) dig_P[7]) / 32768.0;
    mPressure = (p + (var1 + var2 + ((float)dig_P[6])) / 16.0) / 100;

    // Humidity offset calculations
    float var_H = (((float)t_fine) - 76800.0);
    var_H = (adc_h - (dig_H[3] * 64.0 + dig_H[4] / 16384.0 * var_H)) * (dig_H[1] / 65536.0 * (1.0 + dig_H[5] / 67108864.0 * var_H * (1.0 + dig_H[2] / 67108864.0 * var_H)));

    mHumidity = var_H * (1.0 -  dig_H[0] * var_H / 524288.0);

    if(mHumidity > 100.0)
    {
        mHumidity = 100.0;
    }
    else if(mHumidity < 0.0)
    {
        mHumidity = 0.0;
    }
}

BME280::Status BME280::status() const
{
    return mStatus;
}

float BME280::pressure() const
{
    return mPressure;
}

float BME280::cTemp() const
{
    return mCTemp;
}

float BME280::fTemp() const
{
    return mFTemp;
}

float BME280::humidity() const
{
    return mHumidity;
}
