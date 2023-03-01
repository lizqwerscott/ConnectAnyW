#ifndef HTTPUTIL_H
#define HTTPUTIL_H

#include <QString>
#include <QJsonObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "data/clipboardmessage.h"

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
    void httpPost(QString address, QJsonObject data);

    void login(QString host, QString username, QString deviceid);
    void addMessage(QString host, QString deviceid, QString clipboardData);
    void updateBaseMessage(QString host, QString deviceid);

public slots:
    void httpPostFinish(QNetworkReply * reply);

signals:
    void loginResult(bool ok);
    void addMessageResult(bool ok);
    void updateBaseMessageResult(bool ok, ClipboardMessage message);

private:
    QString generateUrl(QString host, QString query);
    void handleError(QString json);
private:
    QNetworkAccessManager * m_mgr;
    QJsonValue m_tempValue;
    HttpPostType m_nowType;
    const QString TAG = "HttpUtil:";
};


#endif // HTTPUTIL_H
