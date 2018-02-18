#include "loaderjson.h"

void LoaderJSON::saveJson(const QJsonDocument &doc, const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::WriteOnly);
    file.write(doc.toJson());
}



QJsonDocument LoaderJSON::loadJson(const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(file.readAll());
}



QVector<QPointF> LoaderJSON::getPoints(const QJsonArray &array)
{
    QVector<QPointF> points;
    for (int indPoint = 0; indPoint < array.size();indPoint = indPoint+2){
        float x = array[indPoint].toDouble();
        float y = array[indPoint+1].toDouble();
        points.push_back(QPointF(x, y));
    }
    return points;
}



QJsonArray LoaderJSON::getArray(const QVector<QPointF> &points)
{
    QJsonArray array;
    for (int indPoint = 0; indPoint < points.size(); indPoint++){
        array.push_back(points[indPoint].x());
        array.push_back(points[indPoint].y());
    }
    return array;
}
