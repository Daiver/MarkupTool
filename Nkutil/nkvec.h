#ifndef NK_VECTORHELPERS_H
#define NK_VECTORHELPERS_H

#include <QVector3D>


namespace nkutil {

QVector3D vecOrthogonalTo(QVector3D x);
QVector3D removeOrthComponent(const QVector3D i, const QVector3D y);
QVector3D firstOrthComponent(const QVector3D x);

}

#endif // VECTORHELPERS_H
