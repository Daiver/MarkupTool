#include "Nkutil/nkvec.h"
#include "Nkutil/nkutil.h"
#include <QDebug>
#include <cmath>

namespace nkutil {

QVector3D vecOrthogonalTo(const QVector3D x)
{
    QVector3D y;
    int coord = 0;
    do {
        y = QVector3D(coord == 0, coord == 1, coord == 2);
        ++coord;
    } while (fabs(QVector3D::dotProduct(x, y)) > 1e-24*x.length()
           && coord < 3);
    return y;
}

QVector3D firstOrthComponent(const QVector3D x) {
    if (fabs(x.lengthSquared() - 1) < 1e-20) { /* kind of loose */
        return QVector3D(1, 0, 0);
    }
    return x.normalized();
}

QVector3D removeOrthComponent(const QVector3D i, const QVector3D y) {
    Q_ASSERT_X(nkutil::eq(i.lengthSquared(), 1.0, .1),
                 "nkutil::removeOrthComponent()",
                 "got a vector that is far not normalized;");
    QVector3D z = y - QVector3D::dotProduct(i, y)*i;
    int coord = 0;
    while ((z.length() < 1e-24
            || fabs(QVector3D::dotProduct(i, z)) > 1e-26)
           && coord < 3) {
        z = QVector3D(coord == 0, coord == 1, coord == 2);
        z = z - QVector3D::dotProduct(z, i)*i;
        z = z.normalized();
        ++coord;
    }
    return z;
}

}
