#include "request.h"
namespace PatientMgr{

int Request::make(QUrl url, Requests::RequestType type, const QByteArray* data)
{
    reply.reset( qnam.get(QNetworkRequest(url)) );
    if(type == Requests::RequestType::POST)
        reply.reset( qnam.post(QNetworkRequest(url), *data) );
    else if(type == Requests::RequestType::PUT)
        reply.reset( qnam.put(QNetworkRequest(url), *data) );


    connect(reply.get(), &QIODevice::readyRead, this, &Request::OnHttpReady);

    connect(reply.get(), &QNetworkReply::finished, this, &Request::OnHttpFinish);
    return 1;
}

void Request::OnHttpReady()
{
    if(!buffer)
        buffer = new QByteArray();

     buffer->append(reply->readAll());
}
void Request::OnHttpFinish()
{
    int code = -1;
    if(reply->error())
        code = -1;
    QVariant vcode = reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute);
    if(vcode.isValid())
    {
        code = vcode.toInt();
    }
    emit OnRequestSuccess(buffer, code);
}

}

//#include "request.moc"
