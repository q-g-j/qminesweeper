#include <QFile>

#include "stylesheet.h"

Stylesheet::Stylesheet()
{
    this->initializeStylesheet();
}

void Stylesheet::initializeStylesheet()
{
    QFile smiley                        (":/stylesheet/infobar_smiley.css");
    QFile smiley_won                    (":/stylesheet/infobar_smiley_won.css");
    QFile smiley_lost                   (":/stylesheet/infobar_smiley_lost.css");
    QFile smiley_surprised              (":/stylesheet/infobar_smiley_surprised.css");
    smiley.open                         (QFile::ReadOnly);
    smiley_won.open                     (QFile::ReadOnly);
    smiley_lost.open                    (QFile::ReadOnly);
    smiley_surprised.open               (QFile::ReadOnly);
    this->stylesheet_smiley             = QLatin1String(smiley.readAll());
    this->stylesheet_smiley_won         = QLatin1String(smiley_won.readAll());
    this->stylesheet_smiley_lost        = QLatin1String(smiley_lost.readAll());
    this->stylesheet_smiley_surprised   = QLatin1String(smiley_surprised.readAll());
    smiley.close();
    smiley_won.close();
    smiley_lost.close();
    smiley_surprised.close();
}
