#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "request.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool connected = false;
    QString baseurl;
    int port;

private slots:
    void on_btnProgram_clicked();
    void on_btnPatient_clicked();

    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();
    void on_btnConnect_clicked();

    void OnConnected(QByteArray* data, int code);

private:
    int CheckConnectMessage();
    Ui::MainWindow *ui;
    PatientMgr::Request checkconnected;
};
#endif // MAINWINDOW_H
