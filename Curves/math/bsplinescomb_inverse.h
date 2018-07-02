#ifndef BSPLINESCOMB_INVERSE_H
#define BSPLINESCOMB_INVERSE_H

#include <Curves/math/bspline.h>
#include <Curves/impl/curveinverse_base.h>

namespace CurvesMath {

template<typename Vec>
class BSplinesCombInverse
        : public CurvesImpl::CurveInverseBase<BSplinesCombInverse<Vec>, Vec, float>
{
public:

    using VecType = Vec;
    using ParamType = typename BSplinesComb<Vec>::ParamType;

    BSplinesCombInverse(BSplinesComb<Vec> spline);
    BSplinesCombInverse(int order,
                        QVector<ParamType> knots,
                        QVector<VecType> coeffsSpline,
                        QVector<VecType> coeffsDeriv,
                        QVector<VecType> coeffsDiff2);
    ParamType pointToParam(Vec P) const;

    inline Vec c(ParamType t) const { return s.paramToPoint(t); }
    inline Vec dc(ParamType t) const { return ds.paramToPoint(t); }
    inline Vec d2c(ParamType t) const { return d2s.paramToPoint(t); }

protected:
    BSplinesComb<Vec> s; /* the spline */
    BSplinesComb<Vec> ds; /* its first */
    BSplinesComb<Vec> d2s; /* and second derivative */
};

}

#include <Curves/impl/bsplinescomb_inverse_impl.h>

#endif // BSPLINESCOMB_INVERSE_H
