#include "clipboardmessage.h"

ClipboardMessage::ClipboardMessage(QString _data, QString _type, QString _date) :
    data(_data),
    type(_type),
    date(_date)
{

}

ClipboardMessage::ClipboardMessage() :
    data(""),
    type(""),
    date("")
{

}

QJsonObject ClipboardMessage::convertJson()
{
    QJsonObject clipboard;
    clipboard["data"] = data;
    clipboard["type"] = type;
    clipboard["date"] = date;
    return clipboard;
}
