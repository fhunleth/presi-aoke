#include "Settings.h"
#include <QSettings>

Settings::Settings()
{
}

QString Settings::slidePath() const
{
    return settings_.value("SlidePath", "").toString();
}

void Settings::setSlidePath(const QString &s)
{
    settings_.setValue("SlidePath", s);
}

int Settings::slidesPerTurn() const
{
    return settings_.value("SlidesPerTurn", 5).toInt();
}

void Settings::setSlidesPerTurn(int slides)
{
    settings_.setValue("SlidesPerTurn", slides);
}

