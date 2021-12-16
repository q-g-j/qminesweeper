QT += core gui widgets

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cell.cpp \
    common.cpp \
    difficulty.cpp \
    field.cpp \
    main.cpp \
    mainwindow.cpp \
    timer.cpp

HEADERS += \
    cell.h \
    common.h \
    difficulty.h \
    field.h \
    mainwindow.h \
    timer.h

FORMS += \
    difficulty.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    fonts/NotoSans-CondensedMedium.ttf \
    stylesheet/button_1.css \
    stylesheet/button_2.css \
    stylesheet/button_3.css \
    stylesheet/button_4.css \
    stylesheet/button_5.css \
    stylesheet/button_6.css \
    stylesheet/button_7.css \
    stylesheet/button_8.css \
    stylesheet/button_covered.css \
    stylesheet/button_flag.css \
    stylesheet/button_mine.css \
    stylesheet/button_mine_hit.css \
    stylesheet/button_uncovered.css \
    stylesheet/infobar_smiley.css \
    stylesheet/infobar_smiley_pressed.css \
    stylesheet/infobar_smiley_won.css \
    stylesheet/infobar_smiley_lost.css
