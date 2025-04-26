#include "mainwindow.h"

#include <QApplication>

#include "programmes.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<HealthProgram>("HealthProgram");

    MainWindow w;
    w.show();
    return a.exec();
}
