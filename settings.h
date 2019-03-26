#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>

#define SETTINGS_FILE "/etc/iot.conf"

class Settings
{
public:
    static void setValue(const QString &key, const QVariant &value);
    static QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
};

#endif // SETTINGS_H
