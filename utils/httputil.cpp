#include "httputil.h"
#include <QJsonDocument>
#include <QByteArray>
#include <QtNetwork/QNetworkRequest>
#include <QDebug>
#include <QHostInfo>

#include "data/device.h"

HttpUtil::HttpUtil(QObject * parent, QNetworkAccessManager * mgr) : QObject(parent), m_mgr(mgr)
{
    connect(m_mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpPostFinish(QNetworkReply*)));
}

HttpUtil::~HttpUtil()
{
    delete m_mgr;
}

void HttpUtil::httpPost(QString address, QJsonObject obj)
{
    qDebug() << "Start";
    const QUrl url(address);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    m_mgr->post(request, data);

}

void HttpUtil::login(QString host, QString username, QString deviceid)
{
    QString address = host + "/user/adduser";
    QJsonObject body;
    body["name"] = username;


    Device device(deviceid);

    body["device"] = device.convertJson();

    m_nowType = HttpPostType::Login;
    httpPost(address, body);
}

void HttpUtil::addMessage(QString host, QString deviceid, QString clipboardData)
{
    QString address = host + "/message/addmessage";
    QJsonObject body;

    Device device(deviceid);
    body["device"] = device.convertJson();

    ClipboardMessage clipboard(clipboardData, "text", "");
    body["message"] = clipboard.convertJson();

    m_nowType = HttpPostType::AddClipboard;
    httpPost(address, body);
}

void HttpUtil::updateBaseMessage(QString host, QString deviceid)
{
    QString address = host + "/message/updatebase";
    QJsonObject body;

    Device device(deviceid);

    body["device"] = device.convertJson();

    m_nowType = HttpPostType::UpdateClipboard;
    httpPost(address, body);
}

void HttpUtil::httpPostFinish(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError){
        QString contents = QString::fromUtf8(reply->readAll());
        qDebug() << TAG << contents;
        handleError(contents);
        switch (m_nowType) {
        case HttpPostType::Login:
        {
            bool data = this->m_tempValue.toBool(false);
            emit loginResult(data);
        }
            break;
        case HttpPostType::AddClipboard:
        {
            bool data = this->m_tempValue.toBool(false);
            emit addMessageResult(data);
        }
            break;
        case HttpPostType::UpdateClipboard:
        {
            QString data = m_tempValue["data"].toString("-1");
            QString type = m_tempValue["type"].toString("-1");
            QString date = m_tempValue["date"].toString("-1");

            if (data != "-1" && type != "-1" && date != "-1") {
                emit updateBaseMessageResult(true, ClipboardMessage(data, type, date));
            } else {
                emit updateBaseMessageResult(false, ClipboardMessage());
            }

        }
            break;
        }
    }
    else{
        QString err = reply->errorString();
        qDebug() << TAG << err;
    }
    reply->deleteLater();
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

