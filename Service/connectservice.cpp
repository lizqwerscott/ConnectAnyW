#include "connectservice.h"
#include <QApplication>
#include <QJsonDocument>
#include <QDebug>

ConnectService::ConnectService(QObject * parent, UserInfo * _userInfo, HttpUtil * _httpUtil) :
    QObject(parent),
    m_userInfo(_userInfo),
    m_clipboard(QApplication::clipboard()),
    m_timer(new QTimer(this)),
    m_clipboardData(""),
    m_httpUtil(_httpUtil),
    m_webSocketUtil(new WebSocketUtil(this, _userInfo))
{
    connect(m_timer, SIGNAL(timeout()), this, SLOT(run()));
    connect(m_httpUtil, SIGNAL(addMessageResult(bool)), this, SLOT(addMessageCallBack(bool)));
    connect(m_httpUtil, SIGNAL(updateBaseMessageResult(bool,ClipboardMessage)), this, SLOT(updateBaseCallBack(bool,ClipboardMessage)));

    connect(m_webSocketUtil, SIGNAL(websocketReciveMessage(bool,ClipboardMessage)), this, SLOT(updateBaseCallBack(bool,ClipboardMessage)));

    QString originalText = m_clipboard->text();	     //获取剪贴板上文本信息
    if (originalText != "") {
        this->m_clipboardData = originalText;
    }
}

void ConnectService::start()
{
    m_timer->start(1000);
    m_webSocketUtil->createDataRecvWS();
}

void ConnectService::stop()
{
    m_timer->stop();
}

void ConnectService::run()
{
    // refresh clipboard
    QString originalText = m_clipboard->text();	     //获取剪贴板上文本信息
    if (originalText != "" && originalText != m_clipboardData) {
        updateClipboard(originalText);
        m_httpUtil->addMessage(m_userInfo->defaultHost, m_userInfo->deviceId, m_clipboardData);
    }
    //        m_clipboard->setText("");
    // update server clipboard data
//    m_httpUtil->updateBaseMessage(m_userInfo->defaultHost, m_userInfo->deviceId);

    m_webSocketUtil->reconnect();

    qDebug() << TAG << "clipboardData: " << m_clipboardData;
}

void ConnectService::addMessageCallBack(bool ok)
{
    if (ok) {
        qDebug() << TAG << "add message finish";
    } else {
        qDebug() << TAG << "add message error";
    }
}

void ConnectService::updateBaseCallBack(bool ok, ClipboardMessage message)
{
    if (ok && message.type == "text") {
        updateClipboard(message.data);
        qDebug() << TAG << "update finish: " << message.data;
    } else {
        qDebug() << TAG << "update base is null";
    }
}

void ConnectService::updateClipboard(QString data)
{
    m_clipboardData = data;
    m_clipboard->setText(m_clipboardData);
}
