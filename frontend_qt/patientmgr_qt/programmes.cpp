#include "programmes.h"
#include "ui_programmes.h"

Programmes::Programmes(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Programmes)
{
    ui->setupUi(this);
}

Programmes::~Programmes()
{
    delete ui;
}

void Programmes::on_btnClear_clicked()
{

}


void Programmes::on_btnDelete_clicked()
{

}


void Programmes::on_btnAdd_clicked()
{

}

void Programmes::on_ledSearch_textChanged(const QString &arg1)
{

}

