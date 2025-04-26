#ifndef REQUEST_H
#define REQUEST_H

#include <QUrl>
#include <QtNetwork>
#include <QNetworkAccessManager>
namespace PatientMgr{
namespace Requests
{
    enum RequestType
    {
        GET,
        POST,
        PUT
    };
};
class Request : public QObject{
    Q_OBJECT


    private slots:
        void OnHttpFinish();
        void OnHttpReady();

    private:
        QByteArray* buffer = nullptr;
        QNetworkAccessManager qnam;
        QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> reply;

    public:
        int make(QUrl url, Requests::RequestType type = Requests::RequestType::GET, const QByteArray* data = nullptr);

    signals:
        void OnRequestSuccess(QByteArray* data, int httpcode);
};

}
#endif
