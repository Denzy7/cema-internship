#ifndef PATIENTS_H
#define PATIENTS_H

#include <QDialog>
#include <QDateTime>
#include "programmes.h"

namespace Ui {
class Patients;
}

class Patient
{
    public:
        uint32_t id = 0;
        QString name;
        QDate dob;
        QList<HealthProgram> programmes;

        QJsonObject toJson() const;
        QList<QStandardItem*> toStandardItemRow() const;
        static Patient fromJson(const QJsonObject &obj);

};
Q_DECLARE_METATYPE(Patient)
class Patients : public QDialog
{
    Q_OBJECT

public:
    explicit Patients(QWidget *parent = nullptr);
    ~Patients();

private slots:
    void on_btnClear_clicked();
    void on_btnDelete_clicked();
    void on_btnEdit_clicked();

    void on_btnAdd_clicked();
    void on_btnRefresh_clicked();

    void on_ledSearch_textChanged(const QString &arg1);
    void on_tbl_SelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void OnGetProgrammes(QByteArray* data, int code, PatientMgr::Request* req);
    void OnGetPatients(QByteArray* data, int code, PatientMgr::Request* req);
    void OnPostPatient(QByteArray* data, int code, PatientMgr::Request* req);
    void OnPutPatient(QByteArray* data, int code, PatientMgr::Request* req);

private:
    Ui::Patients *ui;

    Patient selectedItem;
    QStandardItemModel* modelProgrammes;
    QStandardItemModel* model;
    QSortFilterProxyModel proxymodel;

    QList<HealthProgram> programmes;


    QByteArray *postbuffer, *putbuffer;
    PatientMgr::Request getprogrammes, getpatients, postpatient, putpatient;
};

#endif // PATIENTS_H
