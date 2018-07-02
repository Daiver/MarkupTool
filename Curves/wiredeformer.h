#ifndef WIREDEFORMER_H
#define WIREDEFORMER_H

#include <QVector3D>
#include <QMatrix4x4>
#include <cmath>

namespace Wrap {

/* Deformer
 * based on the paper~\cite{singh1998wires}
 * "Wires: a geometric deformation technique" by Singh et Fiume.
 *
 * Here's the gist of it.
 *
 * First, we cover our original surface with curves.
 * Deformation takes place when a curve's manipulated.
 * The authors call the original curve the Reference $R$,
 * and the deformed curve the Wire $W$.
 *
 * Consider the projection of an arbitrary point $P$ of the surface
 * unto the reference curve $R$. They denote by $P_R$ the (least) parameter
 * that minimizes the distance from the curve $R$ to the point $P$.
 * Thus the projection of $P$ unto $R$ is $R(P_R)$.
 *
 * The idea's to bind the point $P$ to the local coordinate frame
 * (the tangent space and its orthogonal complement)
 * of its projection $R(P_R)$. Then we track the movement of that frame:
 * we find the point on $W$ that corresponds to $R(P_R)$, its tangent
 * and normal, and the rotation and translation that make tangents and normals
 * coincide. Have we bound $P$ to the local frame of $R(P_R)$, the deformation
 * should apply the same rotation and translation to $P$!
 *
 * Of course we want the deformation to be rather smooth
 * or at least continuous. And we'd like't to have a local support.
 * To achieve these the authors introduce
 *
 * - the "density function" $f: \mathbb{R}\to[0,1]$,
 *   such that $f(0)=1$, $f(1)=f'(1)=0$,
 * - the radius of the support $r \in\mathbb{R}$
 *   (the points remoted from $R$ more than by $r$ aren't deformed)
 * - the domain curve $D:[a,b]\to\mathbb{R}^d$, which
 *   basically allows to define different $r$ for points
 *   to one "side" of specific span of the curve.
 *
 * These tools basically define a (sort of a) potentials
 * (curve-parameterized families of potentials).
 * The translation and the rotation angle are simply multiplied
 * by such potential.
 *
 *
 * Offset-volume:
 *  $$ F(P) = f(\frac{ \|P - R(P_R)\| }{r}), $$
 * (it's $F(P;R) in the author's notation)
 *
 * With the domain curve, when $P$ and $D(P_D)$ are
 * to the same side of $R(P_R)$:
 *  $$ F(P) = f(\frac{ \|P - R(P_R)\| }{ \| R(P_R) - D(P_D) \| }), $$
 * "To the same side" means $(P - R(P_R), D(P_D) - R(P_R)) > 0$.
 *
 * The effect of manipulating multiple curves simultaneously
 * is calculated as one of the kinds of average of individual deformations.
 */

/* wireDeformation(P, R, TR, W, TW, f)
 *
 * Given pre-computed potential value `F(P)`
 * computes the deformation (that is the diff, not the end result)
 * of some point $P$ caused by a single wire $W$.
 *
 * That's a draft. Another approach could be to find the corresp.
 * affine transform and return QMatrix4x4.
 *
 */
inline QVector3D wireDeformation(
        const QVector3D P,
        const QVector3D refPoint,
        const QVector3D refTangent,
        const QVector3D wirePoint,
        const QVector3D wireTangent,
        const float potential)
{
    const QVector3D i = firstOrthComponent(refTangent);
    const QVector3D j = removeOrthComponent(i, wireTangent);
    const QVector3D k = QVector3D::crossProduct(i, j);

    const QVector3D local = P - refPoint;
    // qDebug() << "local:" << local;
    const QVector3D aligned(
                i[0]*local[0] + i[1]*local[1] + i[2]*local[2],
                j[0]*local[0] + j[1]*local[1] + j[2]*local[2],
                k[0]*local[0] + k[1]*local[1] + k[2]*local[2]);
    // qDebug() << "aligned:" << aligned;
    const float rnorm = refTangent.length();
    const float wnorm = wireTangent.length();
    const float scalecos = QVector3D::dotProduct(i, wireTangent)/rnorm;
    const float scalesin = QVector3D::crossProduct(i, wireTangent).length()/rnorm;
    const QVector3D rotated(
                scalecos*aligned[0] - scalesin*aligned[1],
                scalesin*aligned[0] + scalecos*aligned[1],
                wnorm/rnorm*aligned[2]);
    // qDebug() << "rotated:" << rotated;
    const QVector3D realigned(
                i[0]*rotated[0] + j[0]*rotated[1] + k[0]*rotated[2],
                i[1]*rotated[0] + j[1]*rotated[1] + k[1]*rotated[2],
                i[2]*rotated[0] + j[2]*rotated[1] + k[2]*rotated[2]);
    // qDebug() << "realigned:" << realigned;
    // qDebug() << "going to translate by" << (wirePoint - refPoint);
    const QVector3D deformed = wirePoint + realigned;
    const QVector3D def = potential*(deformed - P);
    // qDebug() << "deformation:" << def;
    // qDebug() << "deformed:" << deformed;
    return def;
}

class DefaultBlender {
public:
    DefaultBlender(const float m,
                   const float eps=1e-10);
    /* resets the accumulated values */
    void reset();
    /* takes $\mathrm{def}_i$ and mixes it in*/
    void add(QVector3D def);
    /* yields a weighted combination of $\mathrm{def}_i,\ i=\overline{1,n}$ */
    QVector3D result() const;
protected:
    /* The exponent $m$
     *
     * $m=0$ corresponds to simple average,
     * large $m$ results in bias towards the largest deformation,
     * negative $m$ in bias towards small deformations.
     */
    const float m;
    const float eps; /* the minimal norm of an individual deformation */
    float accMagnitude;
    QVector3D accVec;
};

struct Wire {
    virtual QVector3D deformation(const QVector3D P) const = 0;
};

template <typename Blender>
void wireDeform(
        const QVector<QVector3D> points,
        const QVector<Wire*> wires,
        /* passing an object thus for c++
         * doesn't allow floats as non-type template
         * parameters and we want *user*
         * to specify parameters like the exponent $m$
         */
        Blender blender,
        QVector<QVector3D> &out);

QVector<QVector3D> wireDeform(
        const QVector<QVector3D> points,
        const QVector<Wire*> wires,
        const float m);

}

#endif // WIREDEFORMER_H
