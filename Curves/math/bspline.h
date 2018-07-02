#ifndef BSPLINE_H
#define BSPLINE_H

#include <QDebug>
#include <QVector>
#include <QVector3D>
#include <functional>

#include <Nkutil/nkutil.h>
#include <Nkutil/nkmat.h>
#include <Curves/impl/curve_fit.h>
#include <Curves/impl/curve_unif_iter.h>
#include <Nkutil/nkopt.h>

/*
 * B-Splines and B-spline representation of splines
 * ================================================
 *
 * TLDR:
 *
 * 1. We deal with piecewise polynomial functions/curves (=splines)
 *
 * 2. B-Splines are sorta "hump"-functions or "bells" or "hats" used
 *    to blend control points s.t. the curve
 *    goes smoothly following the shape of the control
 *    polygon formed by control points.
 *
 * 3. The BSpline class computes these humps and related stuff.
 *
 * 4. The BSplinesComb deals with splines (pp. functions)
 *    represented as combinations of these "humps".
 *
 * 5. BSplines built on the same knot vector always sum up to unity;
 *    thus if you consider a combination of control points
 *    with bsplines as coefficients it will be
 *    a **convex combination** of these control points;
 *    that's good for blending;
 *
 * 6. BSplines have an "order" which defines how many nearest
 *    controlpoints influence any specific point of the curve;
 *    The higher the order the wider the support of these humps
 *    and the more the curve changes when user "drags" any given
 *    control point.
 *
 * Please refer to:
 *
 * - Carl de Boor, "Splinefunktionen",
 * - Carl de Boor, "A Practical guide to splines, revised ed.",
 * - Carl de Boor, "On calculating B-Splines".
 * - Carl de Boor, lecture notes on approximation theory, http://pages.cs.wisc.edu/~deboor/887/notes.pdf
 *
 * Also for algos (and shitty notation) see:
 * - Piegl, The NURBS Book
 *
 * A shorter, simpler, and more applied intro could be:
 * - http://ftp.cs.wisc.edu/Approx/bsplbasic.pdf
 *
 * Those who need a math TLDR could check out this survey:
 * - ftp://ftp.cs.wisc.edu/Approx/survey76.pdf
 *
 * More on divided differences:
 * - ftp://ftp.cs.wisc.edu/Approx/dvdsurvey.pdf
 */

namespace CurvesMath
{

/* See de Boor: "On calculating B-Splines"
 * and Schoenberg: "Cardinal spline interpolation and spline functions"
 * where they derive condition numbers for uniform knot vectors.
 * It is shown that conditions grow at least exponentionally
 * with the order and claim specifically
 * that "on a 7-digit machine... it limits us to the splines of
 * relatively low order, say less than 20".
 *
 * For that reason we will use (static,
 * stack-allocated if the object's such)
 * std::array of some fixed size.
 */
#define BSPLINE_MAX_ORDER 40
#define BSPLINE_BUFSIZE (1 + BSPLINE_MAX_ORDER)

/* The purpose of this helper class is
 * to compute the basis functions on knot vector $knots$
 * at given time $t$.
 * The basis functions are locally-supported.
 * That means, that only $order + 1$ basis functions
 * are non-zero at any given $t$.
 *
 * Thus after computation:
 * - span is the index of the first basis function not vanishing at $t$
 * - data[0], ..., data[order] are the values of B_{span}, ..., B_{span+order} at $t$.
 */
class BSpline
{
public:
    BSpline(int order, QVector<float> knots, float t);
    inline int first() const /* the index of first non-vanishing basis function */
    { return rightmost - order; }
    inline int last() const /* the index of the last non-vanishing bspline */
    { return rightmost; }
    /* value(i) = B_i(t) for i in [first()..last)) */
    inline float operator ()(int i) const
    {
        if (i < first() || last() < i) {
            return 0.0;
        }
        return data[i - first()];
    }

    template<typename ParSeq1, typename ParSeq2>
    static nkmat::Matrix<float> collocationMatrix(int order, ParSeq1 knots, ParSeq2 params);

private:
    const int rightmost;
    const int order;
    std::array<float, BSPLINE_BUFSIZE> data; /* basis functions values at t */
};

/* Finds the index of the first basis function
 * not vanishing at $t$
 */
int findRightmostBSpline(int order, QVector<float> knots, float t);

QVector<float> createKnotsBernstein(int nCoeffs);
QVector<float> createKnotsUniform(int order, int nCoeffs);
QVector<float> createKnotsAveraged(int order, QVector<float> knots);
template<typename Vec>
QVector<float> createKnotsChordlengths(int order, QVector<Vec> editpoints);

/* Now that we have B-Splines ${ B_{i,k} }$ -- the basis in the space
 * of piecewise-order-k-polynomials  --
 * we can define B-Spline Curves in vector space $V$:
 * Any combination $\sum_i \alpha_i B_{i,k}$
 * with a finite sequence $\alpha$
 * is called a
 * "Spline curve of order $k$ in B-Spline form on knot vector $t$"
 */

template<typename Vec=QVector3D>
class BSplinesComb
{
public:
    typedef BSplinesComb<Vec> CurveType;
    typedef Vec VecType;
    typedef float ParamType;

    /* Create a spline defined in B-form */
    BSplinesComb(int order, const QVector<float> &_knots,
                   const QVector<Vec> &controlVertices);
    static BSplinesComb<Vec> createBernstein(const QVector<Vec> &controlVertices);
    static BSplinesComb<Vec> interpolatePoints(int _order, const QVector<Vec> &editpoints);
    static void interpolatePoints(int _order, const QVector<Vec> &editpoints,
                                            QVector<float> &editparams,
                                            QVector<float> &_knots,
                                            QVector<Vec> &_coeffs);

    BSplinesComb();

    bool isValid() const;
    ParamType minParam() const;
    ParamType maxParam() const;
    CurvesImpl::CurveUniformIterator<CurveType> sample(int nPoints, ParamType from, ParamType to) const;
    CurvesImpl::CurveUniformIterator<CurveType> sample(int nPoints) const;
    QVector<VecType> sampledPoints(int nPoints) const;
    QVector<VecType> sampledPoints(int nPoints, ParamType from, ParamType to) const;
    void visitPoints(int nPoints, std::function<void(Vec) > visitor) const;
    Vec paramToPoint(ParamType t) const;
    Vec paramToTangent(ParamType t) const;
    ParamType pointToParam(VecType P) const;
    QVector<Vec> controlPoints() const { return _coeffs; }
    QVector<float> knots() const { return _knots; }
    int order() const { return _order; }

    BSplinesComb<Vec> derivativeFunction() const;

    OptimizationState fitToData(QVector<Vec> points,
                                float lr=.01);
    template<class ParameterAllocator=UniformParamAllocation<CurveType, Vec> >
    OptimizationState fitToData(QVector<Vec> points,
                                QVector<Vec> tangents,
                                float lr=.01,
                                float pointLossWeight=.5);

protected:
    inline Vec coeff(int i) const {
            i = std::max(0, std::min(i, _coeffs.size() - 1));
            if (i < 0 || _coeffs.size() <= i) {
                 /* returning zero;
                  * 'tis a useful convention
                  * performance-wise as it allows sometimes
                  * not to put unneeded stuff into _coeffs vector;
                  */
                return Vec();
            }
            return _coeffs[i];
    }
    inline ParamType knot(int i) const {
        if (_knots.size() == 0)
            return 0;
        i = std::max(0, std::min(i, _knots.size() - 1));
        return _knots[i];
    }
    Vec derivCoef(int i) const;
    int findRightmostBSpline(ParamType t) const;
    QVector<Vec> _coeffs;
    QVector<ParamType> _knots;
    int _order;

private:
    static bool isNondecreasing(QVector<ParamType> _knots);
    static bool isOfOrder(int order, QVector<ParamType> _knots);
};

}
/* Templates implementation */

#include "Curves/impl/bspline_fit_impl.h"
#include "Curves/impl/bspline_impl.h"
#include "Curves/impl/bspline_interpolate_impl.h"


#endif // BSPLINE_H
