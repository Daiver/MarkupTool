#ifndef BSPLINE_IMPL_H
#define BSPLINE_IMPL_H

#include "Curves/math/bspline.h" /* for autocompletion */
#include <Curves/math/bsplinescomb_inverse.h>

namespace CurvesMath {

template<typename Vec>
BSplinesComb<Vec>::BSplinesComb(
              int order,
              const QVector<float> &knots,
              const QVector<Vec> &controlPoints)
    : _knots(knots),
      _coeffs(controlPoints),
      _order(order)
{
    Q_ASSERT_X(order <= BSPLINE_MAX_ORDER,
          "BSplineCurve()",
          "order can't be larger than BSPLINE_MAX_ORDER.");
    Q_ASSERT_X(isOfOrder(order, knots),
                 "BSplineCurve()",
                 "A knot shall not be repeated more than `order+1` times");
    Q_ASSERT_X(isNondecreasing(knots),
                 "BSplineCurve()",
                 "The knot-vector should be non-decreasing");

    // Q_ASSERT(order + controlPoints.size() + 1 == knots.size());
}

template<typename Vec>
BSplinesComb<Vec> BSplinesComb<Vec>::createBernstein(const QVector<Vec> &controlPoints)
{
    int order = controlPoints.size() - 1;
    auto knots = createKnotsBernstein(controlPoints.size());
    return BSplinesComb<Vec>(order, knots, controlPoints);
}

template<typename Vec>
BSplinesComb<Vec>::BSplinesComb()
{
    /* empty; this allows to put bspline on stack or into an object without
     * passing controlpoints and knots vectors
     */
}

template<typename Vec>
bool BSplinesComb<Vec>::isValid() const
{
    return true;
    // return _order + _coeffs.size() + 1 == _knots.size();
}

template<typename Vec>
typename BSplinesComb<Vec>::ParamType BSplinesComb<Vec>::minParam() const
{
    return knot(_order); /* sort of... */
}

template<typename Vec>
typename BSplinesComb<Vec>::ParamType BSplinesComb<Vec>::maxParam() const
{
    return knot(_knots.size() - _order - 1); /* temporarily */
}

template<typename Vec>
QVector<Vec> BSplinesComb<Vec>::sampledPoints(int nPoints) const
{
    typedef QVector<BSplinesComb<Vec>::VecType> vecs;

    if (!isValid())
        return vecs(0);
     vecs pts;
     pts.reserve(nPoints);
     visitPoints(nPoints, [&pts](Vec P) { pts.append(P); }); /* who cares */
     return pts;
}

/* BOILERPLATE!!! GET RID OF IT AND USE CRTP OR MIXINS OR WHATEVER! */
template<typename Vec>
void BSplinesComb<Vec>::visitPoints(int nPoints, std::function<void (Vec)> visitor) const
{
    if (!isValid())
        return;
    ParamType n = nPoints - 1;
    for (int i = 0; i < nPoints; ++i) {
        ParamType t = (n - i)/n*minParam() + i/n*maxParam();
        Vec P = paramToPoint(t);
        visitor(P);
    }
}

template<typename Vec>
int BSplinesComb<Vec>::findRightmostBSpline(BSplinesComb<Vec>::ParamType t) const
{
    return findRightmostBSpline(_order, _knots, t);
}

template<typename Vec>
bool BSplinesComb<Vec>::isNondecreasing(QVector<BSplinesComb<Vec>::ParamType> knots)
{
    for (int i = 1; i < knots.size(); ++i)
        if (knots[i] < knots[i - 1])
            return false;
    return true;
}

template<typename Vec>
bool BSplinesComb<Vec>::isOfOrder(int order, QVector<BSplinesComb::ParamType> knots)
{
    /* TODO: Check no site in knots occurs more than `order` times */
    return true;
}

template<typename Vec>
Vec BSplinesComb<Vec>::paramToPoint(BSplinesComb<Vec>::ParamType t) const
{
    // Q_ASSERT_X(isValid(),
    //              "BSPlineCurve::eval()",
    //              "BSplineCurve: calling .eval() on a curve"
    //               " that is not in valid state");
    auto basis = BSpline(_order, _knots, t);
    Vec result;
    for (int i = basis.first(); i <= basis.last(); ++i) {
        result += basis(i)*coeff(i);
    }
    return result;
}

template<typename Vec>
typename BSplinesComb<Vec>::ParamType
BSplinesComb<Vec>::pointToParam(Vec P) const
{
    BSplinesCombInverse<Vec> inv(*this);
    auto const parm = inv.pointToParam(P);
    return parm;
}

template<typename Vec>
CurvesImpl::CurveUniformIterator<BSplinesComb<Vec> >
BSplinesComb<Vec>::sample(int nPoints,
                          BSplinesComb<Vec>::ParamType from,
                          BSplinesComb<Vec>::ParamType to) const
{
    return CurvesImpl::CurveUniformIterator<BSplinesComb<Vec> >(this, nPoints, from, to);
}

template<typename Vec>
CurvesImpl::CurveUniformIterator<BSplinesComb<Vec> >
BSplinesComb<Vec>::sample(int nPoints) const
{
    return sample(nPoints, minParam(), maxParam());
}

template<typename Vec>
inline Vec BSplinesComb<Vec>::derivCoef(int i) const {
        /* Splinefunktionen, Seite 63, Gleichung (6.19). */
        const int k = _order;
        const ParamType denom = (knot(i+k) - knot(i))/k;
        const Vec num = coeff(i) - coeff(i-1);
        if (std::abs(denom) > nkutil::EPS) {
                return num/denom;
        }
        return Vec();
}

template<typename Vec>
Vec BSplinesComb<Vec>::paramToTangent(ParamType t) const
{
    const int k = _order;
    const BSpline B(k-1, _knots, t);
    Vec T;
    for (int i = B.first(); i <= B.last(); ++i) {
        if (std::abs(B(i)) > nkutil::EPS) {
            T += derivCoef(i) * B(i);
        }
    }
    return T;
}

template<typename Vec>
BSplinesComb<Vec> BSplinesComb<Vec>::derivativeFunction() const
{
    const QVector<ParamType> &knots = this->_knots; /* knots are the same */
    const int order = std::max(0, this->order() - 1);
    const int nNewCoeffs = this->_coeffs.size(); /* I feel like they should get fewer but don't see which to remove */
    QVector<Vec> coeffs; /* just the coeffs change */
    coeffs.reserve(nNewCoeffs);

    for (int i = 0; i < nNewCoeffs; ++i) {
        coeffs.append(derivCoef(i));
    }

    return BSplinesComb<Vec>(order, knots, coeffs);
}

/* TEMPORARY YAY */
template<typename Vec>
QVector<float> createKnotsChordlengths(/* TODO */ int order, QVector<Vec> editpoints)
{
    const int nCoeffs = editpoints.size();
    const int nKnots = nCoeffs;
    QVector<float> knots(nKnots);
    const float n = nKnots - 1;

    knots.first() = 0;

    float totalLength = 0.;
    for (int i = 0; i < editpoints.size() - 1; ++i) {
        const float len = (editpoints[i + 1] - editpoints[i]).length();
        knots[i + 1] = totalLength + len;
        totalLength += len;
    }
    for (int i = 0; i < editpoints.size() - 1; ++i) {
        knots[i + 1] /= totalLength;
    }

    knots.last() = 1;
    return knots;
}



} /* namespace */
#endif // BSPLINE_IMPL_H
