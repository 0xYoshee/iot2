#include "settings.h"

void Settings::setValue(const QString &key, const QVariant &value)
{
    QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
    settings.setValue(key, value);
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue)
{
    QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
    return settings.value(key, defaultValue);
}
