#ifndef DEVICE_H
#define DEVICE_H

#include <QJsonObject>

class Device
{
public:
    Device(QString _id);

public:
    QJsonObject convertJson();

public:
    QString id;
    #ifdef Q_OS_WIN32
    QString type = "Window";
    #endif
    #ifdef  Q_OS_LINUX
    QString type = "Linux";
    #endif

    QString hostName;
};

#endif // DEVICE_H
