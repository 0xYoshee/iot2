#include "display.h"

Display::Display(char *device, quint8 addr, QObject *parent) :
    QObject(parent)
{
    bus = i2c_init(device, addr);

    if (bus == -10) {
        mEnable = false;
    }
    else{
        ssd1306Init(SSD1306_SWITCHCAPVCC);
        ssd1306ClearScreen(LAYER0 | LAYER1) ;
        ssd1306DrawXBM(50, 15, start_width, start_height, start_bits, WHITE);
        ssd1306Refresh();
        mEnable = true;
    }
}

void Display::clear()
{
    ssd1306ClearScreen(LAYER0 | LAYER1) ;
    ssd1306Refresh();
}

bool Display::enable() const
{
    return mEnable;
}
