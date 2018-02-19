#ifndef BODY_H
#define BODY_H

#include "QPointF"
#include "QVector"
#include "QGraphicsView"
#include "landmark.h"

struct Part
{
    void addPoint (Landmark* point, int indInsert);
    void loadPoint (Landmark* point, int indInsert);
    void deletePoint(int indPoint);
    void update();
    int pointsSize() const;

    QGraphicsPathItem *pathUp = nullptr;
    QGraphicsPathItem *pathDown = nullptr;
    QList<Landmark*> points;

    QVector<QPointF> corner;
    QVector<QPointF> up;
    QVector<QPointF> down;
};

struct Body
{
    Body();

    Part* getActivedPart();

    QVector<Part> parts;
    int indActived = 0;
};

#endif // BODY_H
