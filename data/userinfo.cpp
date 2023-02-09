#include "userinfo.h"
#include <QUuid>

UserInfo::UserInfo(QString _username, QString _deviceid, QString _defaultHost) :
    userName(_username),
    deviceId(_deviceid),
    defaultHost(_defaultHost)
{

}

UserInfo::UserInfo()
{

}
