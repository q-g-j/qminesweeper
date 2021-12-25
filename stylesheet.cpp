#include <QFile>

#include "stylesheet.h"

Stylesheet::Stylesheet()
{
    this->initializeStylesheet();
}

void Stylesheet::initializeStylesheet()
{
    QFile file_button_common            (":/stylesheet/button_common.css");
    QFile file_button_flag              (":/stylesheet/button_flag.css");
    QFile file_button_mine              (":/stylesheet/button_mine.css");
    QFile file_button_mine_hit          (":/stylesheet/button_mine_hit.css");
    QFile file_button_pressed           (":/stylesheet/button_pressed.css");
    QFile file_button_revealed          (":/stylesheet/button_revealed.css");
    QFile file_button_unrevealed        (":/stylesheet/button_unrevealed.css");
    QFile file_button_1                 (":/stylesheet/button_1.css");
    QFile file_button_2                 (":/stylesheet/button_2.css");
    QFile file_button_3                 (":/stylesheet/button_3.css");
    QFile file_button_4                 (":/stylesheet/button_4.css");
    QFile file_button_5                 (":/stylesheet/button_5.css");
    QFile file_button_6                 (":/stylesheet/button_6.css");
    QFile file_button_7                 (":/stylesheet/button_7.css");
    QFile file_button_8                 (":/stylesheet/button_8.css");
    QFile smiley                        (":/stylesheet/infobar_smiley.css");
    QFile smiley_won                    (":/stylesheet/infobar_smiley_won.css");
    QFile smiley_lost                   (":/stylesheet/infobar_smiley_lost.css");
    QFile smiley_surprised              (":/stylesheet/infobar_smiley_surprised.css");
    file_button_common.open             (QFile::ReadOnly);
    file_button_flag.open               (QFile::ReadOnly);
    file_button_mine.open               (QFile::ReadOnly);
    file_button_mine_hit.open           (QFile::ReadOnly);
    file_button_pressed.open            (QFile::ReadOnly);
    file_button_revealed.open           (QFile::ReadOnly);
    file_button_unrevealed.open         (QFile::ReadOnly);
    file_button_1.open                  (QFile::ReadOnly);
    file_button_2.open                  (QFile::ReadOnly);
    file_button_3.open                  (QFile::ReadOnly);
    file_button_4.open                  (QFile::ReadOnly);
    file_button_5.open                  (QFile::ReadOnly);
    file_button_6.open                  (QFile::ReadOnly);
    file_button_7.open                  (QFile::ReadOnly);
    file_button_8.open                  (QFile::ReadOnly);
    smiley.open                         (QFile::ReadOnly);
    smiley_won.open                     (QFile::ReadOnly);
    smiley_lost.open                    (QFile::ReadOnly);
    smiley_surprised.open               (QFile::ReadOnly);
    this->stylesheet_button_common      = QLatin1String(file_button_common.readAll());
    this->stylesheet_button_flag        = QLatin1String(file_button_flag.readAll());
    this->stylesheet_button_mine        = QLatin1String(file_button_mine.readAll());
    this->stylesheet_button_mine_hit    = QLatin1String(file_button_mine_hit.readAll());
    this->stylesheet_button_pressed     = QLatin1String(file_button_pressed.readAll());
    this->stylesheet_button_revealed    = QLatin1String(file_button_revealed.readAll());
    this->stylesheet_button_unrevealed  = QLatin1String(file_button_unrevealed.readAll());
    this->stylesheet_button_1           = QLatin1String(file_button_1.readAll());
    this->stylesheet_button_2           = QLatin1String(file_button_2.readAll());
    this->stylesheet_button_3           = QLatin1String(file_button_3.readAll());
    this->stylesheet_button_4           = QLatin1String(file_button_4.readAll());
    this->stylesheet_button_5           = QLatin1String(file_button_5.readAll());
    this->stylesheet_button_6           = QLatin1String(file_button_6.readAll());
    this->stylesheet_button_7           = QLatin1String(file_button_7.readAll());
    this->stylesheet_button_8           = QLatin1String(file_button_8.readAll());
    this->stylesheet_smiley             = QLatin1String(smiley.readAll());
    this->stylesheet_smiley_won         = QLatin1String(smiley_won.readAll());
    this->stylesheet_smiley_lost        = QLatin1String(smiley_lost.readAll());
    this->stylesheet_smiley_surprised   = QLatin1String(smiley_surprised.readAll());
    file_button_common.close();
    file_button_flag.close();
    file_button_mine.close();
    file_button_mine_hit.close();
    file_button_pressed.close();
    file_button_revealed.close();
    file_button_unrevealed.close();
    file_button_1.close();
    file_button_2.close();
    file_button_3.close();
    file_button_4.close();
    file_button_5.close();
    file_button_6.close();
    file_button_7.close();
    file_button_8.close();
    smiley.close();
    smiley_won.close();
    smiley_lost.close();
    smiley_surprised.close();
}
