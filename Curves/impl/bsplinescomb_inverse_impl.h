#ifndef BSPLINESCOMB_INVERSE_IMPL_H
#define BSPLINESCOMB_INVERSE_IMPL_H

#include <Curves/math/bsplinescomb_inverse.h>

namespace CurvesMath {

template<typename Vec>
BSplinesCombInverse<Vec>::BSplinesCombInverse(BSplinesComb<Vec> spline)
{
    this->s = spline;
    this->ds = s.derivativeFunction();
    this->d2s = ds.derivativeFunction();
}

template<typename Vec>
BSplinesCombInverse<Vec>::BSplinesCombInverse(int order,
                    QVector<ParamType> knots,
                    QVector<VecType> coeffsSpline,
                    QVector<VecType> coeffsDeriv,
                    QVector<VecType> coeffsDiff2)
{
    this->s = BSplinesComb<Vec>(order, knots, coeffsSpline);
    this->ds = BSplinesComb<Vec>(std::max(0, order - 1), knots, coeffsDeriv);
    this->d2s = BSplinesComb<Vec>(std::max(0, order - 2), knots, coeffsDiff2);
}

template<typename Vec>
typename BSplinesCombInverse<Vec>::ParamType
BSplinesCombInverse<Vec>::pointToParam(Vec P) const
{
    float argMin = this->s.minParam();
    float min = std::numeric_limits<float>::infinity();
    auto knots = s.knots();
    auto cps = s.controlPoints();
    const auto order = s.order();
    /* the number's found experimentally;
     * here's the logic: the order of the curve
     * shall be close to the upper bound of the number
     * of splittings required because
     * the curve is piecewise polynomial of the degree < ORDER
     * thus it and its derivatives change direction no more
     * often than ORDER times...
     * for some reason it ain't working with ORDER nor ORDER + 1
     * but it seems to with ORDER + 3...
     */
    const auto intervalsPerSpan = order + 3;
    const ParamType n = intervalsPerSpan - 1;
    for (int span = order; span < knots.size() - 1; ++span) {
        for (int j = 0; j < n; ++j) {
            const ParamType j1 = j + 1;
            const auto T0 = knots[span];
            const auto T1 = knots[span + 1];
            if (T1 - T0 < nkutil::EPS) continue;
            const auto t0 = (n - j)/n*T0 + j/n*T1;
            const auto t1 = (n - j1)/n*T0 + j1/n*T1;

            const auto arg = CurvesImpl::CurveInverseBase<BSplinesCombInverse<Vec>, Vec, float>
                    ::pointToParam(P, t0, t1);
            const auto obj = (P - s.paramToPoint(arg)).lengthSquared();
            if (obj <= min) {
                min = obj;
                argMin = arg;
            }
        }
    }
    return argMin;
}

}

#endif // BSPLINESCOMB_INVERSE_IMPL_H
