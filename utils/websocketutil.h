#ifndef WEBSOCKETUTIL_H
#define WEBSOCKETUTIL_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>

#include "data/userinfo.h"
#include "data/clipboardmessage.h"

class WebSocketUtil : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketUtil(QObject *parent, UserInfo * _userInfo);
public slots:
    /*-<创建websocket连接 */
    void createDataRecvWS();

    /*-<socket建立成功后，触发该函数 */
    void onConnected();
    /*-<收到Sev端的数据时，触发该函数 */
    void onTextReceived(QString msg);
    /*-<socket连接断开后，触发该函数 */
    void onDisConnected();

    void onSslErrors(const QList<QSslError> &errors);
    /*-<websocket周期重连函数 */
    void reconnect();
signals:
    void websocketReciveMessage(bool res, ClipboardMessage message);
private:
    /*-<websocket类 */
    QWebSocket *m_ws;
    /*-<websocket连接状态，连接成功：true；断开：false */
    bool connectStatus;
    /*-<周期重连Timer */
    QTimer *m_websocketTimer;

    UserInfo * m_userInfo;
};

#endif // WEBSOCKETUTIL_H
