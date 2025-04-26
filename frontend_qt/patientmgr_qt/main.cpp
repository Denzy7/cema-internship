#include "mainwindow.h"

#include <QApplication>

#include "programmes.h"
#include "patients.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<HealthProgram>("HealthProgram");
    qRegisterMetaType<Patient>("Patient");

    MainWindow w;
    w.show();
    return a.exec();
}
