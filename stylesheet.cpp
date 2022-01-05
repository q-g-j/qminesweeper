#include <QFile>

#include "stylesheet.h"

Stylesheet::Stylesheet()
{
    this->initializeStylesheet();
}

void Stylesheet::initializeStylesheet()
{
    QFile smiley                (":/stylesheet/infobar_smiley.css");
    QFile smiley_won            (":/stylesheet/infobar_smiley_won.css");
    QFile smiley_lost           (":/stylesheet/infobar_smiley_lost.css");
    QFile smiley_surprised      (":/stylesheet/infobar_smiley_surprised.css");
    QFile digital_0             (":/stylesheet/digital_0.css");
    QFile digital_1             (":/stylesheet/digital_1.css");
    QFile digital_2             (":/stylesheet/digital_2.css");
    QFile digital_3             (":/stylesheet/digital_3.css");
    QFile digital_4             (":/stylesheet/digital_4.css");
    QFile digital_5             (":/stylesheet/digital_5.css");
    QFile digital_6             (":/stylesheet/digital_6.css");
    QFile digital_7             (":/stylesheet/digital_7.css");
    QFile digital_8             (":/stylesheet/digital_8.css");
    QFile digital_9             (":/stylesheet/digital_9.css");
    smiley.open                 (QFile::ReadOnly);
    smiley_won.open             (QFile::ReadOnly);
    smiley_lost.open            (QFile::ReadOnly);
    smiley_surprised.open       (QFile::ReadOnly);
    digital_0.open              (QFile::ReadOnly);
    digital_1.open              (QFile::ReadOnly);
    digital_2.open              (QFile::ReadOnly);
    digital_3.open              (QFile::ReadOnly);
    digital_4.open              (QFile::ReadOnly);
    digital_5.open              (QFile::ReadOnly);
    digital_6.open              (QFile::ReadOnly);
    digital_7.open              (QFile::ReadOnly);
    digital_8.open              (QFile::ReadOnly);
    digital_9.open              (QFile::ReadOnly);
    this->smiley                = QLatin1String(smiley.readAll());
    this->smiley_won            = QLatin1String(smiley_won.readAll());
    this->smiley_lost           = QLatin1String(smiley_lost.readAll());
    this->smiley_surprised      = QLatin1String(smiley_surprised.readAll());
    this->digital_0             = QLatin1String(digital_0.readAll());
    this->digital_1             = QLatin1String(digital_1.readAll());
    this->digital_2             = QLatin1String(digital_2.readAll());
    this->digital_3             = QLatin1String(digital_3.readAll());
    this->digital_4             = QLatin1String(digital_4.readAll());
    this->digital_5             = QLatin1String(digital_5.readAll());
    this->digital_6             = QLatin1String(digital_6.readAll());
    this->digital_7             = QLatin1String(digital_7.readAll());
    this->digital_8             = QLatin1String(digital_8.readAll());
    this->digital_9             = QLatin1String(digital_9.readAll());
    smiley.close();
    smiley_won.close();
    smiley_lost.close();
    smiley_surprised.close();
    digital_0.close();
    digital_1.close();
    digital_2.close();
    digital_3.close();
    digital_4.close();
    digital_5.close();
    digital_6.close();
    digital_7.close();
    digital_8.close();
    digital_9.close();
}
