#ifndef LOADERJSONBODY_H
#define LOADERJSONBODY_H

#include "loaderjson.h"
#include "body.h"

namespace LoaderJSON
{
    ShapeFace getBody(const QJsonDocument &doc);
    QJsonDocument createJson(const ShapeFace *body);
    QJsonObject getInvisibleSegment(const Segment &segment);
    QList<QVector<int>> getInvisibleSegment(QJsonArray &obj);
}

#endif // LOADERJSONBODY_H
