#include "connectservice.h"

ConnectService::ConnectService(QObject * parent, UserInfo * _userInfo) :
    QThread(parent),
    m_userInfo(_userInfo)
{

}

void ConnectService::run()
{

}
