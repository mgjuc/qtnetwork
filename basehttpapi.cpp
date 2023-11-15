#include "basehttpapi.h"
#include <QDebug>
#include <QEventLoop>
#include <QMessageBox>
#include <QCoreApplication>

BaseHttpAPI::BaseHttpAPI(QObject* parent)
    : QObject(parent)
{
    QObject::connect(&_networkAccessMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(serviceRequestFinished(QNetworkReply*)));
}

BaseHttpAPI::~BaseHttpAPI()
{
}

void BaseHttpAPI::get(const QString& url)
{
    _httpRequest.setUrl(QUrl(url));
    QNetworkReply* reply = _networkAccessMgr.get(_httpRequest);
}

void BaseHttpAPI::lambget(const QString& url)
{
    _httpRequest.setUrl(QUrl(url));
    QNetworkReply* reply = _networkAccessMgr.get(_httpRequest);

    //reply必须是值传递
    QObject::connect(reply, &QNetworkReply::finished, [=, &reply] {
        qDebug()<<"from lamb...";
        if (reply == nullptr) {
            qDebug()<<QString("reply not exist");
            return;
        }
        qDebug()<<reply;
        int statuscode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug()<<statuscode<<reply->readAll();
//        QJsonParseError err_rpt;
//        QJsonDocument  doc = QJsonDocument::fromJson(reply->readAll(), &err_rpt);
//        if(err_rpt.error == QJsonParseError::NoError || doc.isNull()){
//            qDebug() << "JSON格式错误";
//            return;
//        }
        reply->deleteLater();
        QCoreApplication::quit();
    });
}

void BaseHttpAPI::synGet(const QString& url)
{
    qDebug() << url;
    _httpRequest.setUrl(QUrl(url));
    QNetworkReply* reply = _networkAccessMgr.get(_httpRequest);
    QEventLoop eventLoop;
    connect(&_networkAccessMgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish
    disconnect(&_networkAccessMgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
}

void BaseHttpAPI::synPost(const QString& url, const QByteArray& data)
{
    qDebug() << url << QString(data);
    _httpRequest.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    _httpRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    _httpRequest.setUrl(QUrl(url));
    QNetworkReply* reply = _networkAccessMgr.post(_httpRequest, data);
    QEventLoop eventLoop;
    connect(&_networkAccessMgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish
    disconnect(&_networkAccessMgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
}

void BaseHttpAPI::post(const QString& url, const QByteArray& data)
{
    _httpRequest.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    _httpRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    _httpRequest.setUrl(QUrl(url));
    QNetworkReply* reply = _networkAccessMgr.post(_httpRequest, data);
}

void BaseHttpAPI::serviceRequestFinished(QNetworkReply* reply)
{
    qDebug() << "serviceRequestFinished" ;
    qDebug()<<reply;
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() == QNetworkReply::NoError)
    {
        _url = reply->url().url();
        _reply = reply;
        _data = reply->readAll();
        _statusCode = statusCode;
//        qDebug() << "reply data:" << _reply;
//        qDebug() << "reply data:" << _data;
//        qDebug() << "reply data:" << _statusCode;
    }
    else
    {
        _reply = nullptr;
        _data = QByteArray();
        _statusCode = 0;
        qDebug()<< reply->url()<< reply->errorString();
    }

    emit sigRequestFinished(reply->url().toString());
    reply->deleteLater();

//    QCoreApplication::quit();
}
