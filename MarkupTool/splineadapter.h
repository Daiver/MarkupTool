#ifndef SPLINEADAPTER_H
#define SPLINEADAPTER_H

#include "interpolation.h"
#include "QPointF"
#include "QVector"
#include "QPainterPath"

class SplineAdapter
{
public:
    SplineAdapter();

    void setPoints(const QVector<QPointF> &points);
    QPainterPath getPath(const double &step = 1) const;
    QPainterPath getPath(const double &xStart, const double &xEnd, const double &step = 1) const;
    QPointF operator ()(const double &x) const;

private:
    alglib::spline1dinterpolant spline;
    alglib::real_1d_array X;
    alglib::real_1d_array Y;
    int countNode = 0;
};

#endif // SPLINEADAPTER_H
