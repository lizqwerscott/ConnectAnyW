#include "httputil.h"
#include <QJsonDocument>
#include <QByteArray>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QDebug>

HttpUtil::HttpUtil(QObject * parent, QNetworkAccessManager * mgr) : QObject(parent), m_mgr(mgr)
{

}

HttpUtil::~HttpUtil()
{
    delete m_mgr;
}

void HttpUtil::httpPost(QString address, QJsonObject obj, HttpPostType type)
{
    qDebug() << "Start";
    const QUrl url(address);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = m_mgr->post(request, data);

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if(reply->error() == QNetworkReply::NoError){
            QString contents = QString::fromUtf8(reply->readAll());
            qDebug() << TAG << contents;
            handleError(contents);
            switch (type) {
            case HttpPostType::Login:
                {
                    bool data = this->m_tempValue.toBool(false);
                    emit loginResult(data);
                }
                break;
            case HttpPostType::AddClipboard:
                break;
            case HttpPostType::UpdateClipboard:
                break;
            }
        }
        else{
            QString err = reply->errorString();
            qDebug() << TAG << err;
        }
        reply->deleteLater();
    });
}

void HttpUtil::login(QString host, QString username, QString deviceid)
{
    QString address = host + "/user/adduser";
    QJsonObject body;
    body["name"] = username;

    QJsonObject device;
    device["id"] = deviceid;

    body["device"] = device;

    this->httpPost(address, body, HttpPostType::Login);
}

void HttpUtil::handleError(QString json)
{
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8(), &jsonError);

    if (document.isNull() || (jsonError.error != QJsonParseError::NoError)) {
        qDebug() << TAG << "Handle error bug";
        return;
    }

    if (!document.isObject()) {
        qDebug() << TAG << "documnet is not object";
        return;
    }

    QJsonObject object = document.object();
    int code = object["code"].toInt(-1);
    QString msg = object["msg"].toString("-1");

    if (code == -1 || msg == "-1") {
        qDebug() << TAG << "code is -1 or msg is -1";
        return;
    }

    if (code == 200) {
        this->m_tempValue = object["data"];
        qDebug() << TAG << msg;
    } else {
        qDebug() << TAG << msg;
    }

}

