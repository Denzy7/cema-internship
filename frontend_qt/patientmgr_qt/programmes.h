#ifndef PROGRAMMES_H
#define PROGRAMMES_H

#include <QDialog>

namespace Ui {
class Programmes;
}

class Programmes : public QDialog
{
    Q_OBJECT

public:
    explicit Programmes(QWidget *parent = nullptr);
    ~Programmes();

private slots:
    void on_btnClear_clicked();

    void on_btnDelete_clicked();

    void on_btnAdd_clicked();

    void on_ledSearch_textChanged(const QString &arg1);

private:
    Ui::Programmes *ui;
};

#endif // PROGRAMMES_H
