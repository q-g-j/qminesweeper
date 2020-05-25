QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    dialog.cpp \
    difficulty.cpp \
    field.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    cell.h \
    common.h \
    dialog.h \
    difficulty.h \
    field.h \
    mainwindow.h

FORMS += \
    dialog.ui \
    difficulty.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    stylesheet/button_1.qss \
    stylesheet/button_2.qss \
    stylesheet/button_3.qss \
    stylesheet/button_4.qss \
    stylesheet/button_5.qss \
    stylesheet/button_6.qss \
    stylesheet/button_7.qss \
    stylesheet/button_8.qss \
    stylesheet/button_covered.qss \
    stylesheet/button_flag.qss \
    stylesheet/button_mine.qss \
    stylesheet/button_mine_hit.qss \
    stylesheet/button_uncovered.qss
