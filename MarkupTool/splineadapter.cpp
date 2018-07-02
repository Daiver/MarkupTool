#include "splineadapter.h"

BSpline::BSpline(const QVector<QPointF> &points)
{
    QVector<QVector2D> pts;
    for (int indPoint = 0; indPoint < points.size(); indPoint++)
        pts.push_back(QVector2D(points[indPoint]));

    spline = new Curves::CurveBSpline<QVector2D>(pts, 2);
    this->points = points;
}

QPointF BSpline::getPoint(const float &x)
{
    QVector2D vec = spline->paramToPoint(x);
    return vec.toPointF();
}

QPainterPath BSpline::getPath(const float &step)
{
    return getPath(spline->minParam(), spline->maxParam(), step);
}

QPainterPath BSpline::getPath(const float &xStart, const float &xEnd, const float &step)
{
    QPainterPath path;
    path.moveTo(getPoint(xStart));

    for (float x = xStart + step; x <= xEnd; x = x + step){
        QPointF point = getPoint(x);
        path.lineTo(point);
    }

    return path;
}

float BSpline::getParam(const QPointF &point)
{
    QVector2D vec(point);
    return spline->pointToParam(vec);
}

