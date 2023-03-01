#include "websocketutil.h"
#include <data/device.h>
#include <QJsonDocument>

WebSocketUtil::WebSocketUtil(QObject *parent, UserInfo * _userInfo)
    : QObject{parent},
      m_ws(Q_NULLPTR),
      connectStatus(false),
      m_websocketTimer(new QTimer()),
      m_userInfo(_userInfo)
{
}

void WebSocketUtil::createDataRecvWS()
{
    if(m_ws == Q_NULLPTR){
        m_ws = new QWebSocket();
        qDebug()<<"create websocket!";
        connect(m_ws, SIGNAL(connected()), this, SLOT(onConnected()));
        connect(m_ws, SIGNAL(disconnected()), this, SLOT(onDisConnected()));
        connect(m_ws, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextReceived(QString)));

        connect(m_websocketTimer, SIGNAL(timeout()), this, SLOT(reconnect()));
        QString url = "ws://" + m_userInfo->defaultHost + ":8687/clipboard";
        qDebug() << "url: " << url;
        m_ws->open(QUrl(url));
    }
}

void WebSocketUtil::onConnected()
{
    qDebug() << "DataReveive websocket is already connect!";
    connectStatus = true;
//    m_websocketTimer->stop();
    qDebug() << "Address：" << m_ws->peerAddress();

    // 发送基础设备信息包
    QJsonObject body;

    Device device(m_userInfo->deviceId);

    body["type"] = "init";
    body["device"] = device.convertJson();
    m_ws->sendTextMessage(QString(QJsonDocument(body).toJson()));
}

void WebSocketUtil::onTextReceived(QString msg)
{
    qDebug() << "----------------websocket data-----------------";
    qDebug() << msg;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(msg.toUtf8().data());
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string "<< msg.toUtf8().data();
    }
    QJsonObject jsonObject = jsonDocument.object();

    QString data = jsonObject["data"].toString("-1");
    QString type = jsonObject["type"].toString("-1");
    QString date = jsonObject["date"].toString("-1");

    if (data != "-1" && type != "-1" && date != "-1") {
        emit websocketReciveMessage(true, ClipboardMessage(data, type, date));
    } else {
        emit websocketReciveMessage(false, ClipboardMessage());
    }
}

void WebSocketUtil::onDisConnected()
{
    qDebug()<<"DataReceive websocket is disconnected!!!";
    connectStatus = false;
    /*-<当连接失败时，触发重连计时器，设置计数周期为1秒 */
    //    m_websocketTimer->start(3000);
}

void WebSocketUtil::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    m_ws->ignoreSslErrors();
}

void WebSocketUtil::reconnect()
{
    if (!connectStatus) {
        qDebug()<<"try to reconnect!";
        m_ws->abort();
        m_ws->open(QUrl("ws://" + m_userInfo->defaultHost + ":8687/clipboard"));
    }
}
