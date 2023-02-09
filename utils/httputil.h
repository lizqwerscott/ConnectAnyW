#ifndef HTTPUTIL_H
#define HTTPUTIL_H

#include <QString>
#include <QJsonObject>
#include <QtNetwork/QNetworkAccessManager>

enum HttpPostType
{
    Login,
    AddClipboard,
    UpdateClipboard
};

class HttpUtil : public QObject
{
    Q_OBJECT
public:
    HttpUtil(QObject * parent, QNetworkAccessManager * mgr);
    ~HttpUtil();
public:
    void httpPost(QString address, QJsonObject data, HttpPostType type);

    void login(QString host, QString username, QString deviceid);

signals:
    void loginResult(bool ok);

private:
    void handleError(QString json);

private:
    QNetworkAccessManager * m_mgr;
    QJsonValue m_tempValue;
    const QString TAG = "HttpUtil:";
};


#endif // HTTPUTIL_H
