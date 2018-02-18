#ifndef LOADERJSON_H
#define LOADERJSON_H

#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonArray"
#include "QVector"
#include "QPointF"
#include "QFile"

namespace LoaderJSON
{
    void saveJson(const QJsonDocument &doc, const QString &fileName);
    QJsonDocument loadJson(const QString &fileName);
    QVector<QPointF> getPoints(const QJsonArray &array);
    QJsonArray getArray(const QVector<QPointF> &points);
}

#endif // LOADERJSON_H
