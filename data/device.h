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
    QString type;
    QString hostName;
};

#endif // DEVICE_H
