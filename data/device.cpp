#include "device.h"
#include <QHostInfo>

Device::Device(QString _id) :
    id(_id),
    hostName(QHostInfo::localHostName())
{

}

QJsonObject Device::convertJson()
{
    QJsonObject device;
    device["id"] = id;
    device["type"] = type;
    device["name"] = hostName;
    return device;
}
