#ifndef NKUTIL_H
#define NKUTIL_H

#include <QVector3D>
#include <cmath>

namespace nkutil {

const double EPS = 1e-16;
const double EPS_SQ = 1e-32;

/* A subroutine for computing the sum
 * of a parameter pack elements */

// template<typename Vec>
// Vec sum(Vec vec)
// {
//     return vec;
// }
//
// template<typename Vec, typename... Vecs>
// Vec sum(Vec first, Vecs rest)
// {
//     return first = sum(rest);
// }

template<typename V1=float, typename V2=V1>
bool eq(V1 v1, V2 v2, float eps=1e-20)
{
    return fabs(v2 - v1) < eps;
}

}

#endif // UTILITY_H
