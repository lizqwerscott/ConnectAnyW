#ifndef CONNECTSERVICE_H
#define CONNECTSERVICE_H

#include <QClipboard>
#include <QTimer>

#include <data/userinfo.h>
#include <utils/httputil.h>

class ConnectService : public QObject
{
    Q_OBJECT
public:
    explicit ConnectService(QObject * parent, UserInfo * _userInfo, HttpUtil * _httpUtil);

public:
    void start();
    void stop();

public slots:
    void run();
    void addMessageCallBack(bool ok);
    void updateBaseCallBack(bool ok, ClipboardMessage message);
private:
    void updateClipboard(QString data);
private:
    UserInfo * m_userInfo;
    QClipboard * m_clipboard;
    QTimer * m_timer;
    QString m_clipboardData;

    HttpUtil * m_httpUtil;

    const QString TAG = "ConnectService:";
};

#endif // CONNECTSERVICE_H
