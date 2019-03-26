#ifndef DEFINES_H
#define DEFINES_H

#define RELAY1 "relay1"
#define RELAY2 "relay2"
#define RELAY3 "relay3"
#define RELAY4 "relay4"
#define RELAY5 "relay5"
#define CAMERA "cam1"

#define DEV_TEMP "/dev/i2c-1"
#define DEV_CAM1 "/dev/video1"
//#define CAM2_DEV "/dev/ttyS2"

//#define BOT_TOKEN ""

#define PIN_DHT       1
//#define PIN_RF433    11
#define PIN_RELAY1   25
#define PIN_RELAY2   24
#define PIN_RELAY3   23
#define PIN_RELAY4   22
#define PIN_RELAY5   21
//#define PIN_SIREN    23
//#define PIN_SCRLIGHT 21

//#define SENSOR_SCAN_TIME 10

#define BOOL_STATE(state) state == true ? "1" : "0"
#define BOOL_TOGGLE(value, state) state=="on" ? value=true : (state=="1" ? value=true : (state=="2" ? value=!value : value=false) )

#endif // DEFINES_H
