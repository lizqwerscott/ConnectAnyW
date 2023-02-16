#ifndef CLIPBOARDMESSAGE_H
#define CLIPBOARDMESSAGE_H

#include <QJsonObject>

class ClipboardMessage
{
public:
    ClipboardMessage(QString data, QString type, QString date = "");
    ClipboardMessage();
public:
    QJsonObject convertJson();
public:
    QString data;
    QString type;
    QString date;
};

#endif // CLIPBOARDMESSAGE_H
