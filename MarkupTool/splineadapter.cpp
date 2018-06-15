#include "splineadapter.h"

SplineAdapter::SplineAdapter()
{

}



void SplineAdapter::setPoints(const QVector<QPointF> &points)
{
    std::vector<double> x;
    std::vector<double> y;
    countNode = points.size();

    for (int indPoint = 0; indPoint < points.size(); indPoint++){
        x.push_back((double)points[indPoint].x());
        y.push_back((double)points[indPoint].y());
    }

    X.setcontent(x.size(), &(x[0]));
    Y.setcontent(y.size(), &(y[0]));

    //alglib::spline1dbuildlinear(AX, AY, spline);
    //alglib::spline1dbuildcubic(X, Y, x.size(), 2, 0.0, 2, 0.0, spline);
    //alglib::spline1dbuildakima(X, Y, spline);
    //alglib::spline1dbuildmonotone(X, Y, spline);
    alglib::spline1dbuildcatmullrom(X, Y, spline);
}



QPainterPath SplineAdapter::getPath(const double &step) const
{
    QPainterPath path;

    double minX = X[0];
    double maxX = X[countNode-1];
    double minY = Y[0];
    double maxY = Y[countNode-1];


    if (minX > maxX){
        double tMaxX = maxX;
        double tMaxY = maxY;

        maxX = minX;
        maxY = minY;
        minX = tMaxX;
        minY = tMaxY;
    }

    path.moveTo(minX, minY);

    for (double x = minX+step; x <= maxX; x = x + step){
        double y = alglib::spline1dcalc(spline,x);
        path.lineTo(x, y);
    }

    return path;
}



QPointF SplineAdapter::operator ()(const double &x) const
{
    QPointF point(x, alglib::spline1dcalc(spline,x));
    return point;
}



QPainterPath SplineAdapter::getPath(const double &xStart, const double &xEnd, const double &step) const
{
    QPainterPath path;

    double minX = X[0];
    double maxX = X[countNode-1];
    double minY = Y[0];
    double maxY = Y[countNode-1];

    double tXStart = xStart;
    double tXEnd = xEnd;


    if (minX > maxX || ((minX < maxX) && (xStart > xEnd))){
        double tMaxX = maxX;
        double tMaxY = maxY;

        maxX = minX;
        maxY = minY;
        minX = tMaxX;
        minY = tMaxY;

        if (xStart > xEnd){
            double tX = tXStart;
            tXStart = tXEnd;
            tXEnd = tX;
        }
    }

    path.moveTo(tXStart, alglib::spline1dcalc(spline,tXStart));

    for (double x = tXStart+step; x <= tXEnd; x = x + step){
        double y = alglib::spline1dcalc(spline,x);
        path.lineTo(x, y);
    }

    return path;
}
