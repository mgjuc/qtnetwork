#ifndef BASEHTTPAPI_H
#define BASEHTTPAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
class BaseHttpAPI : public QObject
{
    Q_OBJECT

public:
    BaseHttpAPI(QObject* parent = nullptr);
    ~BaseHttpAPI();
    void get(const QString& url);
    void synGet(const QString& url);
    void synPost(const QString& url, const QByteArray& data);
    void post(const QString& url, const QByteArray& data);

    void lambget(const QString &url);
signals:
    void sigRequestFinished(QString url);

public slots:
    void serviceRequestFinished(QNetworkReply* reply);

private:
    QNetworkRequest _httpRequest;
    QNetworkAccessManager _networkAccessMgr;
    QNetworkReply* _reply;
    QString _url;
    QByteArray _data;
    int _statusCode;

};



#endif // BASEHTTPAPI_H


