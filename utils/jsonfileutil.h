#ifndef JSONFILEUTIL_H
#define JSONFILEUTIL_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

class JsonFileUtil
{
public:
    JsonFileUtil(const QString& filepath);

    QJsonObject loadJson();
    bool saveJson();

    QJsonObject getJson();

    void writeJson(const QString key, bool value);
    void writeJson(const QString key, int value);
    void writeJson(const QString key, double value);
    void writeJson(const QString key, QString value);
    void writeJson(const QString key, bool* array, int length);
    void writeJson(const QString key, int* array, int length);
    void writeJson(const QString key, double* array, int length);
    void writeJson(const QString key, QJsonObject object);
    QString toString();

private:
    QJsonObject json;
    QString m_filepath;

};

#endif // JSONFILEUTIL_H
