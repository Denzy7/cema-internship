#include "programmes.h"
#include "ui_programmes.h"

#include "mainwindow.h"


#include <QMessageBox>
#include <QJsonDocument>
Programmes::Programmes(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Programmes)
{
    ui->setupUi(this);

    model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({"ID", "Name", "Description"});

    proxymodel.setSourceModel(model);
    proxymodel.setFilterKeyColumn(1);
    proxymodel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxymodel.setSortCaseSensitivity(Qt::CaseInsensitive);

    ui->tblProgrammes->setModel(&proxymodel);

    connect( &getprogrammes,
            &PatientMgr::Request::OnRequestSuccess,
            this, &Programmes::OnGetProgrammes);
    connect( &postprogram,
            &PatientMgr::Request::OnRequestSuccess,
            this, &Programmes::OnPostProgram);
    connect( &putprogram,
            &PatientMgr::Request::OnRequestSuccess,
            this, &Programmes::OnPutProgram);

    connect(ui->tblProgrammes->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &Programmes::on_tbl_SelectionChanged);

    postbuffer = new QByteArray();
    putbuffer = new QByteArray();
    ui->tblProgrammes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    on_btnRefresh_clicked();

}

void Programmes::on_tbl_SelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
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

    selectedItem = data.value<HealthProgram>();
    ui->ledName->setText(selectedItem.name);
    ui->ledDesc->setText(selectedItem.desc);
}
Programmes::~Programmes()
{
    getprogrammes.freeBuffer();
    postprogram.freeBuffer();
    putprogram.freeBuffer();
    delete ui;
}

void Programmes::on_btnClear_clicked()
{
    ui->ledName->clear();
    ui->ledDesc->clear();
}


void Programmes::on_btnEdit_clicked()
{
    if(selectedItem.id <= 0)
    {
        QMessageBox::critical(this,
                "No Item Selected",
                "Select and item to edit");
        return;
    }
    selectedItem.name = ui->ledName->text();
    selectedItem.desc = ui->ledDesc->text();
    QJsonDocument doc(selectedItem.toJson());
    MainWindow *mw = qobject_cast<MainWindow *>(parent());

    QUrl url = 
        QUrl::fromUserInput(
                QString("%1/api/programmes/%2/")
                    .arg(mw->baseurl)
                    .arg(selectedItem.id)
                    );
    url.setPort(mw->port);

    putbuffer->clear();
    putbuffer->append(doc.toJson());
    putprogram.make(url, PatientMgr::Requests::RequestType::PUT, putbuffer);
    ui->btnEdit->setEnabled(false);
    ui->lblStatus->setText("Sending to server...");
}
void Programmes::on_btnDelete_clicked()
{

}


void Programmes::on_btnAdd_clicked()
{
    HealthProgram p;
    p.name = ui->ledName->text();
    p.desc = ui->ledDesc->text();
    QJsonObject o = p.toJson();
    QJsonDocument doc(o);

    MainWindow *mw = qobject_cast<MainWindow *>(parent());

    QUrl url = 
        QUrl::fromUserInput(
                QString("%1/api/programmes/").arg(mw->baseurl)
                );
    url.setPort(mw->port);
    postbuffer->clear();
    postbuffer->append(doc.toJson());
    postprogram.make(url, PatientMgr::Requests::RequestType::POST, postbuffer);

    ui->btnAdd->setEnabled(false);
    ui->lblStatus->setText("Sending to server...");
}

void Programmes::OnGetProgrammes(QByteArray* data, int code)
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
        HealthProgram p = HealthProgram::fromJson(obj);
        model->appendRow(p.toStandardItemRow());
    }

    ui->lblStatus->setText(
            QString("Fetched rows: %1").arg(model->rowCount()));
}

void Programmes::OnPostProgram(QByteArray* data, int code)
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
void Programmes::OnPutProgram(QByteArray* data, int code)
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

void Programmes::on_btnRefresh_clicked()
{
    model->removeRows(0, model->rowCount());
    ui->lblStatus->setText("Fetching data...");

    MainWindow *mw = qobject_cast<MainWindow *>(parent());

    QUrl url = 
        QUrl::fromUserInput(
                QString("%1/api/programmes/").arg(mw->baseurl)
                );
    url.setPort(mw->port);
    getprogrammes.make(url);
}

void Programmes::on_ledSearch_textChanged(const QString &arg1)
{
    proxymodel.setFilterFixedString(arg1);
}

QJsonObject HealthProgram::toJson()const
{
    QJsonObject obj;
    if(id)
        obj["id"] = QJsonValue(static_cast<qint32>(id));
    obj["name"] = name;
    obj["description"] = desc;
    return obj;
}

QList<QStandardItem*> HealthProgram::toStandardItemRow() const
{
    QList<QStandardItem*> row;

    QStandardItem* idItem = new QStandardItem(QString("%1").arg(id));
    row.append(idItem);
    row.append(new QStandardItem(name));
    row.append(new QStandardItem(desc));

    idItem->setData(QVariant::fromValue(*this), Qt::UserRole + 1);

    return row;
}

HealthProgram HealthProgram::fromJson(const QJsonObject &obj)
{
    HealthProgram p;
    p.id = obj.value("id").toVariant().toUInt();
    p.name = obj.value("name").toString();
    p.desc = obj.value("description").toString();
    return p;
}
