#ifndef BODY_H
#define BODY_H

#include "QPointF"
#include "QVector"
#include "QGraphicsView"
#include "landmark.h"

struct Segment
{
    void addPoint (Landmark* point, int indInsert);
    void loadPoint (Landmark* point, int indInsert);
    void setInvisible(int indPoint, bool value);
    void setInvisibleSegment(int indPoint, bool value);
    void clearInvisibleSegment();
    void deletePoint(int indPoint);
    void update();
    Landmark* getEnd();
    Landmark* getStart();
    QPointF getCentral();
    QRect getBox() const;
    int getPointsSize() const;

    void nextInvisibleSegment();
    void pastInvisibleSegment();
    QGraphicsPathItem *pathUp = nullptr;
    QGraphicsPathItem *pathDown = nullptr;
    QList<Landmark*> points;

    QList<QVector<int>> invisibleSegmentsInd;
    int indInvisibleSegment = 0;
    //QVector<int> invisibleSegment;
    QVector<int> invisible;
    QVector<QPointF> corner;
    QVector<QPointF> centers;
    QVector<QPointF> up;
    QVector<QPointF> down;
};

struct ShapeFace
{
    ShapeFace();

    Segment* getActivedPart();
    bool isEmpty() const;
    QVector<Segment> parts;
    int indActived = 0;
};

#endif // BODY_H
