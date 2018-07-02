#ifndef BSPLINE_INTERPOLATE_IMPL_H
#define BSPLINE_INTERPOLATE_IMPL_H

#include <Curves/math/bspline.h>
#include <Nkutil/nkmat.h>

namespace CurvesMath {

template<typename ParSeq1, typename ParSeq2>
::nkmat::Matrix<float> BSpline::collocationMatrix(int order, ParSeq1 knots, ParSeq2 params)
{
    const int nPts = params.size();
    ::nkmat::Matrix<float> colmat(params.size(), params.size());
    for (int i = 0; i < nPts; ++i) {
        BSpline B(order, knots, params[i]);
        for (int j = 0; j < nPts; ++j) { /* TODO: first()..last() */
            colmat(i, j) = B(j);
        }
    }
    return colmat;
}

template<typename Vec>
void BSplinesComb<Vec>::interpolatePoints(int order, const QVector<Vec> &editpoints,
                                            QVector<float> &editparams,
                                            QVector<float> &knots,
                                            QVector<Vec> &coeffs)
{
    const int nEpts = editpoints.size();
    /* TODO: stop reallocating QVectors */
    editparams = createKnotsChordlengths(order, editpoints);
     /* basically you just need each editparam
      * to be in a different nontrivial span;
      * this guarantees
      * 1. Linear independence
      * 2. Spannedness of the collocation matrix;
      *    just imagine what happens underneath:
      *    as you iterate over (nonzero) spans,
      *    from left to right,
      *    what was the leftmost nonzero bspline
      *    on one iteration vanishes on the next;
      *    thus the nonzero part of the row of the collocation matrix
      *    shifts to the right as we go over the rows from up downward;
      *
      * De Boor suggests in his "A Practical Guide to Splines"
      * the following knots:
      */
    knots = createKnotsAveraged(order, editparams);
    auto A = BSpline::collocationMatrix(order, knots, editparams);
    nkmat::Matrix<Vec> rhs(nEpts, 1);
    coeffs.resize(nEpts);
    for (int i = 0; i < nEpts; ++i) {
        rhs(i, 0) = editpoints[i];
    }
    nkmat::solveEliminationNoPivot(A, rhs);
    for (int i = 0; i < nEpts; ++i) {
        coeffs[i] = rhs(i, 0);
    }
}

template<typename Vec>
BSplinesComb<Vec> BSplinesComb<Vec>::interpolatePoints(int order, const QVector<Vec> &editpoints)
{
    QVector<float> editparams;
    QVector<float> knots;
    QVector<Vec> coeffs;
    BSplinesComb<Vec>::interpolatePoints(order, editpoints, editparams, knots, coeffs);
    return BSplinesComb<Vec>(order, knots, coeffs);
}

}
#endif // BSPLINE_INTERPOLATE_IMPL_H
