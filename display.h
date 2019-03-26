#ifndef DISPLAY_H
#define DISPLAY_H

#include <QObject>

#include "ssd1306/ssd1306.h"

#include "images.h"

extern int bus;

class Display : public QObject
{
    Q_OBJECT
public:
    explicit Display(char *device, quint8 addr=0x3c, QObject *parent = nullptr);
    void clear();

    bool enable() const;

signals:

public slots:

private:
    bool mEnable;

};

#endif // DISPLAY_H
