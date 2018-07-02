#ifndef NK_CATMULLROM_H_
#define NK_CATMULLROM_H_

#include <QVector3D>
#include <memory>
#include <functional>
#include <Nkutil/nkutil.h>
#include <Nkutil/curves.h>
#include <Curves/impl/curveinverse_base.h>

namespace CurvesMath {

/* Catmull-Rom spline implementation
 *
 * Refer to~\cite{lengyel2012mathematics}
 *
 * Given the control points $P_0, \ldots, P_n$,
 * the Catmull-Rom Splines interpolate
 * the points $P_1, \ldots, P_{n-1}$
 * with piecewise cubic polynomial curve
 * with tangent at each point $P_i$
 * given by $T_i = \frac12(P_{i+1} + P_{i-1}).
 *
 * In this class we imply the uniform knot vector.
 *
 *
 * TODO:
 * 1. Resampling/downsampling/decimation -- there are
 *    quite a few ideas on how to do it. Not yet sure
 *    what'll do, so it's pending.
 *    Just call me when it's needed.
 * 2. Iterators instead of visitors -- I'd prefer iterators
 *    compatible with <algorithm> and <functional>,
 *    but stl iterators are (mis)designed as if only containers
 *    could have iterators. Our to-be iterators are containerless.
 * 3. Knot insertion, other basic geometric algos -- again,
 *    just bid me do it if it's suddenly needed.
 * 4. Newton and most of helpers are common for all curves,
 *    so I want to separate them into some mix-in or crtp base classes.
 *    Maybe later.
 */


template <typename Vec=QVector3D,
          typename Numeric=float>
class CatmullRom
{
public:
    using ParamType = Numeric; /* [t0(), t1()] global coordinates */
    typedef Numeric SpanCoord; /* \in [0,1]; local coordinates of a point within a span */
    typedef std::tuple<ParamType, ParamType> Span;
    typedef int SpanIndex;
    typedef std::tuple<SpanIndex, SpanCoord> PointInSpan;

    CatmullRom(QVector<Vec> coeffs);

    int nSpans() const;
    Numeric minParam() const;
    Numeric maxParam() const;
    bool isValid() const;
    void visitSpans(std::function<void(Span)> visit) const;
    void visitPointsUnif(int nPoints, std::function<void(Vec)> visit) const;
    void visitPointsUnif(int nPoints, std::function<void(Numeric)> visit) const;
    Vec paramToPoint(const ParamType t) const;
    Vec paramToTangent(const ParamType t) const;
    Vec paramToDiff2(const ParamType t) const;
    PointInSpan paramToPointInSpan(Numeric t) const;
    Vec paramToPoint(const PointInSpan sp) const;
    Vec paramToTangent(const PointInSpan sp) const;
    Vec paramToDiff2(const PointInSpan sp) const;

    QVector<Vec> controlPoints() const;
    void setControlPoints(QVector<Vec> cps);

    ParamType pointToParam(Vec P) const;

protected:
    CatmullRom() { };

private:
    /* A helper that computes the linear combination
     * of control points of the span.
     *
     * This might be replaced
     * with recursive variadic template sum()
     * in some crtp-base or mixin
     * later
     */
    inline
    Vec linComb(int span,
                Numeric c1, Numeric c2,
                Numeric c3, Numeric c4) const;

protected:
    QVector<Vec> coeffs;
};

template<typename Vec,
         typename Numeric>
class CatmullRomInverse
        : public CurvesImpl::CurveInverseBase<CatmullRomInverse<Vec, Numeric>, Vec, Numeric>
        , protected CatmullRom<Vec, Numeric>
{
public:
    using ParamType = Numeric;

    CatmullRomInverse(CatmullRom<Vec, Numeric> spline);

    ParamType pointToParam(Vec P) const;
    Vec c(ParamType t) const;
    Vec dc(ParamType t) const;
    Vec d2c(ParamType t) const;

};


template <typename Vec,
          typename Numeric>
CatmullRom<Vec,Numeric>::CatmullRom(QVector<Vec> controlPoints)
    : coeffs(controlPoints)
{
}

template <typename Vec,
          typename Numeric>
int CatmullRom<Vec,Numeric>::nSpans() const
{
    if (coeffs.size() == 0)
        return 0;
    if (coeffs.size() == 4)
        return 1;
    return coeffs.size() - 3;
}

template <typename Vec,
          typename Numeric>
bool CatmullRom<Vec,Numeric>::isValid() const
{
    return coeffs.size() >= 4;
}

template <typename Vec,
          typename Numeric>
Numeric CatmullRom<Vec,Numeric>::minParam() const { return 0; }

template <typename Vec,
          typename Numeric>
Numeric CatmullRom<Vec,Numeric>::maxParam() const { return nSpans(); }

template <typename Vec,
          typename Numeric>
void CatmullRom<Vec,Numeric>::visitSpans(std::function<void(Span)> visit) const
{
    for (int i = 0; i < nSpans(); ++i) {
        visit(Span(i, i + 1.));
    }
}

/* Yeah, it seems useless,
     * but 1) it doesn't allow you to fuck up with n+/-1,
     * 2) later there will be curvelength-iterator,
     * thus allowing a tradeoff between performance
     * and beauty...
     */
template <typename Vec,
          typename Numeric>
void CatmullRom<Vec,Numeric>::visitPointsUnif(
        int nPoints,
        std::function<void(Numeric)> visit) const
{
    if (!isValid())
        return;
    Numeric n = nPoints - 1;
    for (int i = 0; i <= n; ++i) {
        const Numeric t = (n - i)/n * minParam() + i/n * maxParam();
        visit(t);
    }
}

template <typename Vec,
          typename Numeric>
void CatmullRom<Vec,Numeric>::visitPointsUnif(int nPoints, std::function<void(Vec)> visit) const
{
    // gcc, please inline it before I decide what to do with iterators!
    visitPointsUnif(nPoints, [this, visit](Numeric t){ visit(paramToPoint(t)); });
}

template <typename Vec,
          typename Numeric>
typename CatmullRom<Vec, Numeric>::PointInSpan
CatmullRom<Vec,Numeric>::paramToPointInSpan(Numeric t) const
{
    const int n = coeffs.size();
    t = std::max(t, minParam());
    t = std::min(t, maxParam());
    int span = int(t + 1);
    t = t - (span - 1);
    if (span + 2 >= coeffs.size()) {
        span = coeffs.size() - 3;
        t = 1;
    }
    return PointInSpan(span, t);
}

template <typename Vec,
          typename Numeric>
inline
Vec CatmullRom<Vec,Numeric>::linComb(int span,
                                     Numeric c1, Numeric c2,
                                     Numeric c3, Numeric c4) const
{
    Q_ASSERT_X(coeffs.size() >= 4,
                 "CatmullRom::linComb()",
                 "Called eval() on a spline"
                 " with less than four control points"
                 ", filling with zeroes");
    const Vec a = coeffs[span - 1];
    const Vec b = coeffs[span];
    const Vec c = coeffs[span + 1];
    const Vec d = coeffs[span + 2];

    return c1*a + c2*b + c3*c + c4*d;
}

/* 1. Hope it gets inlined despite the lack of keyword
     * 2. Ain't gonna test nor eval coefficients independent of the curve
     * 3. Though they can be tested by using a single span + \mathbb{R}^4 vector
     */
template <typename Vec,
          typename Numeric>
Vec CatmullRom<Vec,Numeric>::paramToPoint(const PointInSpan sp) const
{

    const Numeric t0 = 1;
    const Numeric t1 = std::get<1>(sp);
    const Numeric t2 = t1*t1;
    const Numeric t3 = t2*t1;
    /* Refer to~\cite{lengyel2012mathematics}
         * TODO: I don't actually remember which of this one
         * and Horner's scheme is more stable; oh, wait, I know
         * it should be the divided differences!
         * whatever, it works this way as well */
    const Numeric c1 = .5*(0    + -1*t1 +  2*t2 + -1*t3);
    const Numeric c2 = .5*(2*t0 + 0     + -5*t2 +  3*t3);
    const Numeric c3 = .5*(0    + 1*t1  +  4*t2 + -3*t3);
    const Numeric c4 = .5*(0    + 0     + -1*t2 +  1*t3);

    return linComb(std::get<0>(sp), c1, c2, c3, c4);
}

template <typename Vec,
          typename Numeric>
QVector<Vec> CatmullRom<Vec,Numeric>::controlPoints() const
{
    return this->coeffs;
}

template <typename Vec,
          typename Numeric>
Vec CatmullRom<Vec,Numeric>::paramToPoint(const Numeric t) const
{
    PointInSpan sp = paramToPointInSpan(t);
    return paramToPoint(sp);
}


template <typename Vec,
          typename Numeric>
Vec CatmullRom<Vec,Numeric>::paramToTangent(const PointInSpan sp) const
{
    const Numeric t = std::get<1>(sp);
    const Numeric dt = 1.0;
    const Numeric dt2 = 2.*t;
    const Numeric dt3 = 3.*t*t;

    const Numeric c1 = .5*(-1*dt +  2*dt2 + -1*dt3);
    const Numeric c2 = .5*( 0*dt + -5*dt2 +  3*dt3);
    const Numeric c3 = .5*( 1*dt +  4*dt2 + -3*dt3);
    const Numeric c4 = .5*( 0*dt + -1*dt2 +  1*dt3);

    return linComb(std::get<0>(sp), c1, c2, c3, c4);
}

template <typename Vec,
          typename Numeric>
Vec CatmullRom<Vec,Numeric>::paramToTangent(const Numeric t) const
{
    PointInSpan sp = paramToPointInSpan(t);
    return paramToTangent(sp);
}

template <typename Vec,
          typename Numeric>
Vec CatmullRom<Vec,Numeric>::paramToDiff2(const PointInSpan sp) const
{
    const Numeric t = std::get<1>(sp);
    const Numeric d2t2 = 2.;
    const Numeric d2t3 = 6.*t;
    const Numeric c1 = .5*( 2*d2t2 + -1*d2t3);
    const Numeric c2 = .5*(-5*d2t2 +  3*d2t3);
    const Numeric c3 = .5*( 4*d2t2 + -3*d2t3);
    const Numeric c4 = .5*(-1*d2t2 +  1*d2t3);

    return linComb(std::get<0>(sp), c1, c2, c3, c4);
}

template <typename Vec,
          typename Numeric>
Vec CatmullRom<Vec,Numeric>::paramToDiff2(const Numeric t) const
{
    PointInSpan sp = paramToPointInSpan(t);
    return paramToDiff2(sp);
}


/* Projecting a point unto the curve
 *
 * NB: here we expect Vec to implement .dotProduct().
 *
 * Finds a projection of a given point P unto the sline.
 * At the moment we're just running Newton's iteration
 * in each span of the curve.
 * It'd be kind of more appropriate to get at least an initial
 * approximation by picking control points closest to P
 * and scanning their adjacent spans.
 * Then we'd need some estimates to be able to filter the spans
 * whose knots lie farther.
 *
 * That's another
 *  TODO: implement some span-rejection technique
 * Also,
 *  TODO: as this algo is basically curve-agnostic
 *        (given visitSpans() and differentiability),
 *        it needs to be separated out. Possibly into a mixin
 */
template<typename Vec,
         typename Numeric>
Numeric CatmullRom<Vec, Numeric>::pointToParam(Vec P) const
{
    CatmullRomInverse<Vec, Numeric> inverseMap(*this);
    auto const parm = inverseMap.pointToParam(P);
    return parm;
}

template<typename Vec,
         typename Numeric>
CatmullRomInverse<Vec, Numeric>::CatmullRomInverse(CatmullRom<Vec, Numeric> spline)
{
    this->coeffs = spline.controlPoints();
}

template<typename Vec,
         typename Numeric>
Numeric CatmullRomInverse<Vec, Numeric>::pointToParam(Vec P) const
{
    Numeric argMin = this->minParam();
    Numeric min = std::numeric_limits<Numeric>::infinity();
    for (int span = 0; span < this->nSpans(); ++span) {
        const auto arg = CurvesImpl::CurveInverseBase<CatmullRomInverse<Vec, Numeric>, Vec, Numeric>
                ::pointToParam(P, Numeric(span), Numeric(span + 1.));
        const auto obj = (P - this->paramToPoint(arg)).lengthSquared();
        if (obj <= min) {
            min = obj;
            argMin = arg;
        }
    }
    return argMin;
}

template<typename Vec,
         typename Numeric>
inline Vec CatmullRomInverse<Vec, Numeric>
::c(CatmullRomInverse<Vec, Numeric>::ParamType t) const
{
    return this->paramToPoint(t);
}

template<typename Vec,
         typename Numeric>
inline Vec CatmullRomInverse<Vec, Numeric>
::dc(CatmullRomInverse<Vec, Numeric>::ParamType t) const
{
    return this->paramToTangent(t);
}

template<typename Vec,
         typename Numeric>
inline Vec CatmullRomInverse<Vec, Numeric>
::d2c(CatmullRomInverse<Vec, Numeric>::ParamType t) const
{
    return this->paramToDiff2(t);
}

}
#endif /* NK_CATMULLROM_H_ */
