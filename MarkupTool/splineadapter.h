#ifndef SPLINEADAPTER_H
#define SPLINEADAPTER_H

#include "QPointF"
#include "QVector"
#include "QPainterPath"
#include "Curves/curve_wrapper.h"
#include "Curves/curvebspline.h"
#include "QVector2D"

class SplineAdapter
{
public:
    virtual QPointF getPoint(const float &x) = 0;
    virtual QPainterPath getPath(const float &step) = 0;
    virtual QPainterPath getPath(const float &xStart, const float &xEnd, const float &step) = 0;
    virtual float getParam(const QPointF &point) = 0;
};

class BSpline : public SplineAdapter
{
public:
    BSpline(const QVector<QPointF> &points);

    QPointF getPoint(const float &x) override;
    QPainterPath getPath(const float &step) override;
    QPainterPath getPath(const float &xStart, const float &xEnd, const float &step) override;
    float getParam(const QPointF &point) override;

private:
    Curves::CurveBSpline<QVector2D> *spline = nullptr;
    QVector<QPointF> points;
};
#endif // SPLINEADAPTER_H
