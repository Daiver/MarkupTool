#ifndef NK_CURVEHELPERS_H
#define NK_CURVEHELPERS_H

#include <QVector3D>
#include <QDebug>


namespace nkutil {

/* An objective for minimzation of
 * Euclidean distance from a point of a curve $C:[a,b]\to H$
 * in an inner product space $H$ (think $$\mathbb{R}^3$$)
 * to a fixed point $P \in H$.
 *
 * $$ J_1(t) = \|C(t) - P\|_2^2 \to \min_{t\in [a,b]}. $$
 *
 * Since in inner product space
 * $$ \|C(t) - P\| = (C(t) - P, C(t) - P) =
 *    (C(t), C(t)) - 2(C(t), P(t)) + \|P\|_2^2, $$
 * where $\|P\|$ is constant and thus can be ommited in optimization objective.
 * Then note $(C(t), C(t)) - 2(C(t), P) = (C(t), C(t) - 2P)$.
 * Thus we could've as well solved the problem
 *
 * $$ J_2(t) = (C(t), C(t) - 2P) \to \min_{t\in [a,b]}. $$
 *
 * If we're to use Newton's method for optimization,
 * we need the first two derivatives of $J_2$.
 * They are easy to compute, given that $C\in C^{(2)}([a,b], H)$:
 *
 * $$ \dot J_2(t)  =   (\dot C(t), C(t) - 2P) + (C(t), \dot C(t)), $$
 * i.e.
 * $$ \dot J_2(t)  = 2 (\dot C(t), C(t) - P), $$
 * $$ \ddot J_2(t) = (\ddot C(t), C(t) - 2P) + (\dot C(t), \dot C(t)). $$
 *
 * TODO: check that you don't need to divide d2Obj by 2!
 *
 * In the signatures below we use the notation:
 *
 * `C` for a point $C(t)$ of the curve,
 * `dC` for the first derivative of the curve $\dot C(t)$,
 * `d2C` for the second derivatve $\ddot C(t)$,
 * `P` for $P$.
 *
 * Likewise, `dObjective...` and `d2Objective...`
 * stand for the derivatives of the objective function.
 */

template <typename Vec=QVector3D>
float inline objectiveCurveToPoint(Vec C,
                                   Vec P)
{
    return .5*Vec::dotProduct(C - 2*P, C);
}

template <typename Vec=QVector3D>
float inline dObjectiveCurveToPoint(Vec C,
                                    Vec dC,
                                    Vec P)
{
   return Vec::dotProduct(dC, C - P);
}

template <typename Vec=QVector3D>
float inline d2ObjectiveCurveToPoint(Vec C,
                                    Vec dC,
                                    Vec d2C,
                                    Vec P)
{
    return Vec::dotProduct(d2C, C - P) + dC.lengthSquared();
}

template<typename Vec=QVector3D>
QVector<Vec> pointsToSegments(QVector<Vec> points)
{
    int nOut = 2*(points.size() - 1);
    if (nOut < 0) nOut = 0;
    QVector<Vec> segments(nOut);
    for(int i = 0; 2*i + 1 < nOut; ++i) {
        segments[2*i] = points[i];
        segments[2*i + 1] = points[i + 1];
    }

    return segments;
}

template<typename Curve, typename Vec=typename Curve::Vec>
QVector<Vec> curveSegments(Curve c, int nPoints)
{
    int nOut = 2*(nPoints - 1);
    if (nOut <= 0) return QVector<Vec>(0);
    QVector<Vec> segments(nOut);
    segments.reserve(nOut);

    c.visitPointsUnif(nPoints, [&](Vec P) {
        if (segments.size() > 1) {
            segments.append(segments.last());
        } else {
            segments.append(P);
        }
        segments.append(P);
    });
    if (segments.size() % 2 != 0) {
        /* #NOASSERT
         * probably doesn't have anything to do with client code,
         * but with some internal error -- so no crash but self-adjust
         */
        qDebug() << "pointsToSegments() doesn't work correctly;"
                    " segments.size() should be even;"
                    " padding with the last element";
        segments.append(segments.last());
    }
    return segments;
}
}

#endif // NK_CURVEHELPERS_H
