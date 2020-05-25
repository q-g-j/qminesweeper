#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QThread>

#include "common.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Common::setRandomSeed();

//    w.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    w.setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    w.show();
    return a.exec();
}
