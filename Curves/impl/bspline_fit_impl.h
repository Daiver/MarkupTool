#ifndef BSPLINE_FIT_IMPL_H
#define BSPLINE_FIT_IMPL_H

#include "Curves/math/bspline.h" /* for autocompletion */

namespace CurvesMath {


#define BSPLINE_FIT_MAX_ITER 1000

template<typename Vec>
OptimizationState BSplinesComb<Vec>::fitToData(QVector<Vec> points,
               float lr)
{
    /* we only optimize the control points so far */
    QVector<ParamType> sites(points.size());
    for (int i = 0; i < points.size(); ++i) {
        sites[i] = pointToParam(points[i]);
    }
    QVector<Vec> upd(_coeffs.size());
    OptimizationState state(lr, 1024);

    for (; !state.done(); state.finishIteration()) {
        double J = 0;
        for (int sampleIdx = 0; sampleIdx < points.size() ; ++sampleIdx) {
            ParamType t = sites[sampleIdx];
            BSpline B(_order, _knots, t);
            /* grad wrt points w/o tangents = B^T \otimes (BC - 2P)
             * with C the control points and P the points to fit to */
            Vec BC;
            for (int j = B.first(); j <= B.last(); ++j) {
                BC += B(j)*coeff(j);
            }
            for (int j = B.first(); j <= B.last(); ++j) {
                upd[j] += 2.0 * B(j) * (BC - points[sampleIdx]);
            }
            J += (BC - points[sampleIdx]).lengthSquared()/points.size();
        }
        double updQty = 0.0;
        for (int j = 0; j < _coeffs.size(); ++j) {
            _coeffs[j] -= state.learningRate()*upd[j];
            updQty += state.learningRate()*state.learningRate()*upd[j].lengthSquared();
            upd[j] = VecType();
        }
        state.setUpdateQty(updQty);
        state.setObjective(J);
    }

    return state;
}

template<typename Vec>
template<class ParameterAllocator>
OptimizationState BSplinesComb<Vec>::fitToData(
        QVector<Vec> points,
        QVector<Vec> tangents,
        float lr,
        float alpha)
{
    /* we only optimize the control points so far */
    QVector<Vec> upd(_coeffs.size());
    OptimizationState state(lr);

    for (; !state.done(); state.finishIteration()) {
        auto pIt = ParameterAllocator(*this, points);
        int sampleIdx = 0;
        double J = 0;
        for (; sampleIdx < points.size() && !pIt.end(); ++sampleIdx, pIt.next()) {
            ParamType t = pIt.get();
            BSpline B(_order, _knots, t);
            Vec pt;
            for (int j = B.first(); j <= B.last(); ++j) {
                pt += B(j)*_coeffs[j];
            }
            for (int j = B.first(); j <= B.last(); ++j) {
                upd[j] += 2.0 * B(j) * (pt - points[sampleIdx]);
            }
            BSpline Btan(_order-1, _knots, t);
            Vec tan;
            for (int j = Btan.first(); j <= Btan.last(); ++j) {
                tan += Btan(j)*derivCoef(j);
            }
            for (int j = Btan.first(); j <= Btan.last(); ++j) {
                upd[j] += 2.0 * Btan(j) * (tan - tangents[sampleIdx]);
            }
            J += alpha*(pt - points[sampleIdx]).lengthSquared()/points.size();
            J += (1.0-alpha)*(tan - tangents[sampleIdx]).lengthSquared()/points.size();
        }
        double updQty = 0.0;
        for (int j = 0; j < _coeffs.size(); ++j) {
            _coeffs[j] -= state.learningRate()*upd[j];
            updQty += state.learningRate()*state.learningRate()*upd[j].lengthSquared();
            upd[j] = VecType();
        }
        state.setUpdateQty(updQty);
        state.setObjective(J);
    }

    return state;
}

}

#endif // BSPLINE_FIT_IMPL_H

