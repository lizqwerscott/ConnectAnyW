#include "jsonfileutil.h"
#include <QFile>
#include <QFileInfo>

JsonFileUtil::JsonFileUtil(const QString& filepath)
{
    this->m_filepath = filepath;
    QFileInfo fileInfo(this->m_filepath);
    if (fileInfo.isFile()) {
        this->loadJson();
    } else {
        this->saveJson();
    }
}

QJsonObject JsonFileUtil::loadJson()
{
    QFile loadFile(this->m_filepath);

    if (!loadFile.open(QIODevice::ReadOnly))
        qDebug() << "Unable to load JSON file";

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if (json_error.error != QJsonParseError::NoError)
        qDebug() << "JSON error!";

    QJsonObject rootObj = jsonDoc.object();
    this->json = rootObj;
    return rootObj;
}

bool JsonFileUtil::saveJson()
{
    QJsonDocument document;
    document.setObject(json);
    QFile file(this->m_filepath);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Fail to save contents to JSON file";
        return false;
    }

    file.write(document.toJson());

    return true;
}

QJsonObject JsonFileUtil::getJson()
{
    return this->json;
}

// NOTE: implicit conversion turns string literal into bool
void JsonFileUtil::writeJson(const QString key, bool value)
{
    json.insert(key, value);
}

void JsonFileUtil::writeJson(const QString key, int value)
{
    json.insert(key, value);
}

void JsonFileUtil::writeJson(const QString key, double value)
{
    json.insert(key, value);
}

// value only support QString
void JsonFileUtil::writeJson(const QString key, QString value)
{
    json.insert(key, QString(value));
}

void JsonFileUtil::writeJson(const QString key, bool* array, int length)
{
    QJsonArray arr;
    for (int i = 0; i < length; i++)
        arr.append(array[i]);
    json.insert(key, arr);
}

void JsonFileUtil::writeJson(const QString key, int* array, int length)
{
    QJsonArray arr;
    for (int i = 0; i < length; i++)
        arr.append(array[i]);
    json.insert(key, arr);
}

void JsonFileUtil::writeJson(const QString key, double* array, int length)
{
    QJsonArray arr;
    for (int i = 0; i < length; i++)
        arr.append(array[i]);
    json.insert(key, arr);
}

void JsonFileUtil::writeJson(const QString key, QJsonObject object)
{
    json.insert(key, object);
}

QString JsonFileUtil::toString()
{
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString str(byteArray);
    return str;
}
