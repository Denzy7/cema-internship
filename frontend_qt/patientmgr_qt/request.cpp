#include "request.h"
namespace PatientMgr{

int Request::make(QUrl url, Requests::RequestType type, const QByteArray* data)
{
    if(buffer)
        buffer->clear();

    QNetworkRequest req(url);
    
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    reply.reset( qnam.get(req) );

    if(type == Requests::RequestType::POST)
        reply.reset( qnam.post(req, *data) );
    else if(type == Requests::RequestType::PUT)
        reply.reset( qnam.put(req, *data) );


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

void Request::freeBuffer()
{
    if(buffer)
        delete buffer;
}
}

//#include "request.moc"
