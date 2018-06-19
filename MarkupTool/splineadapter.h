#ifndef SPLINEADAPTER_H
#define SPLINEADAPTER_H

#include "QPointF"
#include "QVector"
#include "QPainterPath"
#include "Curves/curve_wrapper.h"
#include "QVector2D"

class SplineAdapter
{
public:
    virtual QPointF getPoint(const float &x) = 0;
    virtual QPainterPath getPath(const float &step) = 0;
    virtual QPainterPath getPath(const float &xStart, const float &xEnd, const float &step) = 0;
    virtual float getParam(const QPointF &point) = 0;
};

class CatMullRomSpline : public SplineAdapter
{
public:
    CatMullRomSpline(const QVector<QPointF> &points);

    QPointF getPoint(const float &x) override;
    QPainterPath getPath(const float &step) override;
    QPainterPath getPath(const float &xStart, const float &xEnd, const float &step) override;
    float getParam(const QPointF &point) override;

private:
    Curves::Curve<QVector2D> spline;
    QVector<QPointF> points;
};
#endif // SPLINEADAPTER_H
