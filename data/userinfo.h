#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include <QJsonObject>

class UserInfo
{
public:
    UserInfo(QString _username, QString _deviceid, QString _defaultHost);
    UserInfo();

public:
    QString userName;
    QString deviceId;
    QString defaultHost;
};

#endif // USERINFO_H
