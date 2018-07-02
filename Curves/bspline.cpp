#include <Curves/math/bspline.h>
#include <QDebug>

/* Algo A2.2 from "The NURBS Book"
 * I'll get rid of it later.
 */
CurvesMath::BSpline::BSpline(int order,
                             QVector<float> knots,
                             float t)
    : data(),
    rightmost(findRightmostBSpline(order, knots, t)),
    order(order)
{
    const int i = rightmost;
    Q_ASSERT(knots[i] < knots[i + 1]);
    std::array<float, BSPLINE_BUFSIZE> left;
    std::array<float, BSPLINE_BUFSIZE> right;

    data[0] = 1.0;
    float saved = 0;
    float temp = 0;

    for (int j = 1; j <= order; ++j) {
        left[j] = t - knots[i+1-j];
        right[j] = knots[i+j] - t;
        saved = 0.0;
        for (int r = 0; r < j; ++r) {
            const auto denom = (right[r+1] + left[j-r]);
            Q_ASSERT (std::abs(denom) > nkutil::EPS);
            temp = data[r]/denom;
            data[r] = saved + right[r+1]*temp;
            saved = left[j-r]*temp;
        }
        data[j] = saved;
    }
}

int
CurvesMath::findRightmostBSpline(int order, QVector<float> knots, float t)
{
    const int lo = order;
    const int hi = knots.size() - 1;

    const int right = std::upper_bound(knots.begin() + lo, knots.begin() + hi, t) - knots.begin() - 1;
    if (!(t < knots[right + 1])) {
        const int left = std::lower_bound(knots.begin() + lo, knots.begin() + right, t) - knots.begin() - 1;
        return left;
    }

    return right;
}

/* Temporary: later to be replaced with iterable classes */

QVector<float> CurvesMath::createKnotsBernstein(int nCoeffs)
{
    const int order = nCoeffs - 1;
    const int nKnots = (order + 1) * 2;
    QVector<float> knots(nKnots);
    for (int i = 0; i <= order; ++i) {
        knots[i] = 0.0;
        knots[knots.size()-i-1] = 1.0;
    }
    return knots;
}
QVector<float> CurvesMath::createKnotsUniform(int nCoeffs, int order)
{
    const int nKnots = nCoeffs + order;
    QVector<float> knots(nKnots);
    const float n = nKnots - 1;
    for (int i = 0; i <= n; ++i) {
        knots[i] = float(i)/n;
    }
    return knots;
}

/* A?.? from The-NURBS-Book
 * See the comment bspline_inteprolate_impl.h
 */
QVector<float> CurvesMath::createKnotsAveraged(int order, QVector<float> editParams)
{
    const int n = editParams.size() - 1;
    const int m = n + order + 1;
    const int nKnots = m + 1;
    QVector<float> knots(nKnots);
    for (int i = 0; i <= order; ++i) {
        knots[i] = 0.;
        *(knots.end() - 1 - i) = 1.0;
    }
    for(int i = 1; i <= n - order; ++i) {
        for (int k = i; k < i + order; ++k) {
            knots[i + order] += editParams[k]/order;
        }
    }
    return knots;
}
