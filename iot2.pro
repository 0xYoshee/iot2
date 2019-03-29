QT -= gui
QT += network
QT += serialport
QT += printsupport

CONFIG += c++11 console
CONFIG -= app_bundle

LIBS += -lqmqtt
LIBS += -lwiringPi
LIBS += -lQtDropbox2

LIBS += -lopencv_core
LIBS += -lopencv_imgproc

target.path = /usr/sbin
INSTALLS += target

INCLUDEPATH += $$PWD/lib/mqtt-inc
INCLUDEPATH += $$PWD/lib/dropbox

include($$PWD/lib/QtTelegramBot/QtTelegramBot.pri)

SOURCES += \
        main.cpp \
    ssd1306/ssd1306.c \
    ssd1306/ubuntuMono_8pt.c \
    ssd1306/ubuntuMono_16pt.c \
    ssd1306/ubuntuMono_24pt.c \
    ssd1306/ubuntuMono_48pt.c \
    settings.cpp \
    uartdevice.cpp \
    modem.cpp \
    dht.cpp \
    cameradev.cpp \
    camerathread.cpp \
    pininaction.cpp \
    pinoutaction.cpp \
    display.cpp \
    body.cpp \
    bme280.cpp \
    imageconverter.cpp \
    filesender.cpp \
    motiondetector.cpp \
    webstream.cpp

HEADERS += \
    images.h \
    ssd1306/font.h \
    ssd1306/ssd1306.h \
    ssd1306/ubuntuMono_8pt.h \
    ssd1306/ubuntuMono_16pt.h \
    ssd1306/ubuntuMono_24pt.h \
    ssd1306/ubuntuMono_48pt.h \
    defines.h \
    settings.h \
    uartdevice.h \
    modem.h \
    dht.h \
    cameradev.h \
    camerathread.h \
    pininaction.h \
    pinoutaction.h \
    display.h \
    body.h \
    bme280.h \
    imageconverter.h \
    filesender.h \
    motiondetector.h \
    webstream.h
