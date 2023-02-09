#ifndef CONNECTSERVICE_H
#define CONNECTSERVICE_H

#include <QThread>

#include <data/userinfo.h>

class ConnectService : public QThread
{
public:
    ConnectService(QObject * parent, UserInfo * _userInfo);

public:
    void run();
private:
    UserInfo * m_userInfo;
};

#endif // CONNECTSERVICE_H
