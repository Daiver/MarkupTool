#include "splineadapter.h"

CatMullRomSpline::CatMullRomSpline(const QVector<QPointF> &points)
{
    QVector<QVector2D> pts;
    for (int indPoint = 0; indPoint < points.size(); indPoint++)
        pts.push_back(QVector2D(points[indPoint]));

    spline = Curves::BSplineCurve<QVector2D>::interpolatePoints(2, pts);
    this->points = points;
}

QPointF CatMullRomSpline::getPoint(const float &x)
{
    QVector2D vec = spline.paramToPoint(x);
    return vec.toPointF();
}

QPainterPath CatMullRomSpline::getPath(const float &step)
{
    return getPath(spline.minParam(), spline.maxParam(), step);
}

QPainterPath CatMullRomSpline::getPath(const float &xStart, const float &xEnd, const float &step)
{
    QPainterPath path;
    path.moveTo(getPoint(xStart));

    for (float x = xStart + step; x <= xEnd; x = x + step){
        QPointF point = getPoint(x);
        path.lineTo(point);
    }

    return path;
}

float CatMullRomSpline::getParam(const QPointF &point)
{
    QVector2D vec(point);
    return spline.pointToParam(vec);
}

