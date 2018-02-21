#ifndef SPLINES_H
#define SPLINES_H

#include <QPainterPath>
#include <cmath>

namespace Spline{
    QPainterPath build(const QPolygonF &points);
}
#endif // SPLINES_H
