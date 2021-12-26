#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QString>

class Stylesheet
{
public:
    Stylesheet();

    QString stylesheet_smiley;
    QString stylesheet_smiley_won;
    QString stylesheet_smiley_lost;
    QString stylesheet_smiley_surprised;

    void initializeStylesheet();
};

#endif // STYLESHEET_H
