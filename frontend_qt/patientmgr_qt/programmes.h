#ifndef PROGRAMMES_H
#define PROGRAMMES_H

#include <QDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSortFilterProxyModel>

#include "request.h"
#include <QMetaType>

namespace Ui {
class Programmes;
}


class HealthProgram{
    public:
        uint32_t id = 0;
        QString name;
        QString desc;

        QJsonObject toJson() const;
        QList<QStandardItem*> toStandardItemRow() const;
        static HealthProgram fromJson(const QJsonObject &obj);
};
Q_DECLARE_METATYPE(HealthProgram)


class Programmes : public QDialog
{
    Q_OBJECT

public:
    explicit Programmes(QWidget *parent = nullptr);
    ~Programmes();

private slots:
    void on_btnClear_clicked();

    void on_btnDelete_clicked();
    void on_btnEdit_clicked();

    void on_btnAdd_clicked();
    void on_btnRefresh_clicked();

    void on_ledSearch_textChanged(const QString &arg1);
    void on_tbl_SelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);


    void OnGetProgrammes(QByteArray* data, int code);
    void OnPostProgram(QByteArray* data, int code);
    void OnPutProgram(QByteArray* data, int code);

private:
    Ui::Programmes *ui;

    HealthProgram selectedItem;
    QStandardItemModel* model;
    QSortFilterProxyModel proxymodel;
    QByteArray *postbuffer, *putbuffer;
    PatientMgr::Request getprogrammes, postprogram, putprogram;
};

#endif // PROGRAMMES_H
