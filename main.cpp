#include <QCoreApplication>

#include <QtCore>

#include "display.h"
#include "body.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    bool isDisplay = Settings::value("display/enable", true).toBool();

    Display *display;

    if(isDisplay)
        display = new Display("/dev/i2c-0");

    usleep (3000*1000); // camera init time

    Body body;

    if(isDisplay)
        body.setDisplay(display);

    return a.exec();    
}
