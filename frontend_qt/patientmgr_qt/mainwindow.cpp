#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "programmes.h"
#include <QMessageBox>


#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&checkconnected, 
            &PatientMgr::Request::OnRequestSuccess,
            this, &MainWindow::OnConnected);
}

MainWindow::~MainWindow()
{
    checkconnected.freeBuffer();
    delete ui;
}

void MainWindow::on_btnProgram_clicked()
{
    if(!connected)
    {
        QMessageBox::critical(this,
                "Not connected",
                "Enter address and port and click connect"
                );
        return;
    }
    Programmes programmes(this);
    programmes.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About", "Patient and Program Manager");
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}
void MainWindow::on_actionQuit_triggered()
{
    close();
}
void MainWindow::on_btnConnect_clicked()
{
    connected = false;
    ui->statusbar->showMessage("Connecting...");

    baseurl = ui->ledAddr->text().trimmed() + "/patientmgr";
    port = ui->sbxPort->value();

    QUrl url = 
        QUrl::fromUserInput(
                QString("%1/check").arg(baseurl)
                );
    url.setPort(port);
    if(!url.isValid())
    {
        QMessageBox::critical(this,
                "Invalid URL",
                "Check URL and try again");
        return;
    }
    checkconnected.make(url);
}

void MainWindow::OnConnected(QByteArray* data, int code)
{
    if(code >= 200 && code < 300 && QString::fromUtf8(*data) == "OK")
        connected = true;

    if(connected)
        ui->statusbar->showMessage("Connected successfully");
    else
        ui->statusbar->showMessage("Connection failed!");

}

