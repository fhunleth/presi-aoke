#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings
{
public:
    Settings();

    QString slidePath() const;
    void setSlidePath(const QString &s);

    int slidesPerTurn() const;
    void setSlidesPerTurn(int slides);

private:
    QSettings settings_;
};

#endif // SETTINGS_H
