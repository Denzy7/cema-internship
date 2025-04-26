#include "patients.h"
#include "ui_patients.h"

#include <QMessageBox>
#include <QStyledItemDelegate>
#include <QComboBox>
#include "mainwindow.h"

#define DATEFMTSTR "yyyy-MM-dd"


class ComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ComboBoxDelegate(const QStringList &items, QObject *parent = nullptr)
        : QStyledItemDelegate(parent), m_items(items) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const override
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems(m_items);
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        QString currentText = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int idx = comboBox->findText(currentText);
        if (idx >= 0)
            comboBox->setCurrentIndex(idx);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString selected = comboBox->currentText();
        model->setData(index, selected, Qt::EditRole);
    }

private:
    QStringList m_items;
};


Patients::Patients(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Patients)
{
    ui->setupUi(this);


    model = new QStandardItemModel();
    modelProgrammes = new QStandardItemModel();

    model->setHorizontalHeaderLabels({"ID", "Name", "Date of Birth"});
    modelProgrammes->setHorizontalHeaderLabels({"Name"});

    proxymodel.setSourceModel(model);
    proxymodel.setFilterKeyColumn(1);
    proxymodel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxymodel.setSortCaseSensitivity(Qt::CaseInsensitive);

    ui->tblProgrammes->setModel(modelProgrammes);
    ui->tblPatients->setModel(&proxymodel);

    connect( &getpatients,
            &PatientMgr::Request::OnRequestSuccess,
            this, &Patients::OnGetPatients);
    connect( &postpatient,
            &PatientMgr::Request::OnRequestSuccess,
            this, &Patients::OnPostPatient);
    connect( &putpatient,
            &PatientMgr::Request::OnRequestSuccess,
            this, &Patients::OnPutPatient);
    connect(&getprogrammes,
            &PatientMgr::Request::OnRequestSuccess,
            this, &Patients::OnGetProgrammes);


    connect(ui->tblPatients->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &Patients::on_tbl_SelectionChanged);

    postbuffer = new QByteArray();
    putbuffer = new QByteArray();

    ui->tblPatients->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblProgrammes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    on_btnRefresh_clicked();
}


Patients::~Patients()
{
    getpatients.freeBuffer();
    postpatient.freeBuffer();
    putpatient.freeBuffer();
    delete ui;
}
void Patients::on_btnClear_clicked()
{
    ui->ledName->clear();
    ui->dedDOB->clear();
}


void Patients::on_btnEdit_clicked()
{
    if(selectedItem.id <= 0)
    {
        QMessageBox::critical(this,
                "No Item Selected",
                "Select and item to edit");
        return;
    }
    selectedItem.name = ui->ledName->text();
    selectedItem.dob = ui->dedDOB->date();
    QJsonDocument doc(selectedItem.toJson());
    MainWindow *mw = qobject_cast<MainWindow *>(parent());

    QUrl url = 
        QUrl::fromUserInput(
                QString("%1/api/patient/%2/")
                .arg(mw->baseurl)
                .arg(selectedItem.id)
                );
    url.setPort(mw->port);

    putbuffer->clear();
    putbuffer->append(doc.toJson());
    putpatient.make(url, PatientMgr::Requests::RequestType::PUT, putbuffer);
    ui->btnEdit->setEnabled(false);
    ui->lblStatus->setText("Sending to server...");
}
void Patients::on_btnDelete_clicked()
{

}

void Patients::on_btnAdd_clicked()
{
    Patient p;
    p.name = ui->ledName->text();
    p.dob = ui->dedDOB->date();

    //programmes....
    //
    //
    QJsonObject o = p.toJson();
    QJsonDocument doc(o);

    MainWindow *mw = qobject_cast<MainWindow *>(parent());
    QUrl url = 
        QUrl::fromUserInput(
                QString("%1/api/patient/").arg(mw->baseurl)
                );
    url.setPort(mw->port);
    postbuffer->clear();
    postbuffer->append(doc.toJson());
    postpatient.make(url, PatientMgr::Requests::RequestType::POST, postbuffer);

    ui->btnAdd->setEnabled(false);
    ui->lblStatus->setText("Sending to server...");
    
}

void Patients::OnGetPatients(QByteArray* data, int code, PatientMgr::Request* req)
{
    if(code < 200 || code >= 300)
    {
        QMessageBox::critical(this,
                "Error",
                "Failed to reach API server. Try reconnecting...");

        ui->lblStatus->setText("Fetch failed!");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(*data);
    QJsonArray array = doc.array();
    for(const QJsonValue& value: array)
    {
        QJsonObject obj = value.toObject();
        Patient p = Patient::fromJson(obj);
        model->appendRow(p.toStandardItemRow());
    }

    ui->lblStatus->setText(
            QString("Fetched rows: %1").arg(model->rowCount()));
}


void Patients::OnPostPatient(QByteArray* data, int code, PatientMgr::Request* req)
{
    ui->btnAdd->setEnabled(true);

    if(code < 200 || code >= 300)
    {
        QMessageBox::critical(this,
                "Post Request Failed",
                "Try reconnecting if that fixes the issue...");
        qDebug() << *data;
        ui->lblStatus->setText("Post Request Failed!");
        return;
    }
    ui->lblStatus->setText("Success");
    on_btnRefresh_clicked();
}


void Patients::OnGetProgrammes(QByteArray* data, int code, PatientMgr::Request* req)
{
    if(code < 200 || code >= 300)
    {
        QMessageBox::critical(this,
                "Error",
                "Failed to reach API server. Try reconnecting...");

        ui->lblStatus->setText("Fetch failed!");
        return;
    }

    QStringList proglist;

    QJsonDocument doc = QJsonDocument::fromJson(*data);
    QJsonArray array = doc.array();
    for(const QJsonValue& value: array)
    {
        QJsonObject obj = value.toObject();
        HealthProgram p = HealthProgram::fromJson(obj);
        proglist.append(p.name);
        programmes.append(p);
    }

    ComboBoxDelegate *comboDelegate = new ComboBoxDelegate(proglist, this);
    ui->tblProgrammes->setItemDelegateForColumn(0, comboDelegate);
}

void Patients::OnPutPatient(QByteArray* data, int code, PatientMgr::Request* req)
{
    ui->btnEdit->setEnabled(true);

    if(code < 200 || code >= 300)
    {
        QMessageBox::critical(this,
                "Put Request Failed",
                "Try reconnecting if that fixes the issue...");
        ui->lblStatus->setText("Put Request Failed!");
        qDebug() << *data;
        return;
    }
    ui->lblStatus->setText("Success");
    on_btnRefresh_clicked();

}

void Patients::on_btnRefresh_clicked()
{
    model->removeRows(0, model->rowCount());
    ui->lblStatus->setText("Fetching data...");

    MainWindow *mw = qobject_cast<MainWindow *>(parent());

    QUrl url = 
        QUrl::fromUserInput(
                QString("%1/api/patient/").arg(mw->baseurl)
                );
    url.setPort(mw->port);
    getpatients.make(url);

    QUrl url2 = 
        QUrl::fromUserInput(
                QString("%1/api/programmes/").arg(mw->baseurl));
    url2.setPort(mw->port);
    getprogrammes.make(url2);
}

void Patients::on_ledSearch_textChanged(const QString &arg1)
{
    proxymodel.setFilterFixedString(arg1);
}

QJsonObject Patient::toJson() const
{
    QJsonObject obj;
    if(id)
        obj["id"] = QJsonValue(static_cast<qint32>(id));
    obj["name"] = name; 
    obj["dob"] = dob.toString(); 
    QJsonArray array;
    for(HealthProgram program : programmes)
    {
        QJsonObject pObj;
        pObj["program"] = QJsonValue(static_cast<qint32>(program.id));
        /* fk */
        pObj["program_id"] = QJsonValue(static_cast<qint32>(program.id));
    }
    obj["programmes"] = array;
    return obj;
}
Patient Patient::fromJson(const QJsonObject &obj)
{
    Patient p;
    p.id = obj.value("id").toVariant().toUInt();
    p.name = obj.value("name").toString();
    p.dob = QDate::fromString(
            obj.value("dob").toString(),
            DATEFMTSTR);

    QJsonArray programmes = obj.value("programmes").toArray();
    for (const QJsonValue &val : programmes) {
        HealthProgram prog;
        prog.id = val.toObject().value("program").toInt();
        p.programmes.append(prog);
    }

    return p;
}

QList<QStandardItem*> Patient::toStandardItemRow() const
{
    QList<QStandardItem*> row;

    QStandardItem* idItem = new QStandardItem(QString("%1").arg(id));
    row.append(idItem);

    row.append(new QStandardItem(name));
    row.append(new QStandardItem(dob.toString()));
    idItem->setData(QVariant::fromValue(*this), Qt::UserRole + 1);
    return row;
}

void Patients::on_tbl_SelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected.indexes().isEmpty())
        return;

    QModelIndex idx = proxymodel.mapToSource(selected.indexes().first());
    QStandardItem *item = model->item(idx.row(), 0);

    if (!item)
        return;

    QVariant data = item->data(Qt::UserRole + 1);
    if (!data.isValid())
        return;
    selectedItem = data.value<Patient>();

    modelProgrammes->removeRows(0, modelProgrammes->rowCount());

    MainWindow *mw = qobject_cast<MainWindow *>(parent());

    ui->ledName->setText(selectedItem.name);
    ui->dedDOB->setDate(selectedItem.dob);
    for(HealthProgram p: selectedItem.programmes)
    {
        for(HealthProgram x: programmes)
        {
            if(x.id == p.id)
            {
                QList<QStandardItem*> row;
                row.append(new QStandardItem(x.name));
                modelProgrammes->appendRow(row);
            }
        }

    }
}

#include "patients.moc"
