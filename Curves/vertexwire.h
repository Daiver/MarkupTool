#ifndef VERTEXWIRE_H
#define VERTEXWIRE_H

#include "wiredeformer.h"

namespace Wrap {

#define DEFAULT_DELTA 1e-4

/* VertexWire{Radial, OneSided}
 *
 * This is a generic base for Wires (see wiredeformer.h)
 * that are based on vertices-defined curves (e.g. all kinds of splines).
 * This template takes two parameters:
 * -   class Curve, should implement:
 *     -    Curve(QVector<QVector3D> points),
 *     -    eval(t),
 *     -    evalDeriv(t),
 *     -    project(P) -- returns the parameter $t$ minimizing
 *          the distance from the Curve to P).
 * -   Density: Float to Float
 *     -    A density used to compute the potential.
 *     -    Refer to the paper (where it's named density as well
 *          and is denoted by $f$).
 *     -    Density(0) = 1.
 *     -    Density(R\setminus [0,1)) = {0}.
 * The following routine is implemented:
 * -   Constructor taking curves.
 * -   Constructor taking QVectors of control points.
 * -   deformation(P)
 *
 * To use this template one shall inherit from it,
 * providing specific Curve and Density,
 * and explicitly inherit the constructors with the "using" operator.
 *
 * See wirecatmullrom.h for examples.
 *
 * TODO: deformation(points, out) to reduce the number of calls-through-pointers.
 * TODO: consider inheriting from Curve (instead of leaving public Curve members)
 */

inline float WireDensityLinear(float t) {
    return std::max(0.0, 1.0 - t);
}

template <typename Curve,
          float (*Density)(float) = WireDensityLinear>
class VertexWireRadial : public Wire
{
public:
    VertexWireRadial(const Curve ref, const Curve def, const float r);
    VertexWireRadial(const QVector<QVector3D> ref,
                     const QVector<QVector3D> def,
                     const float r);
// protected:
    const Curve reference;
    const Curve deformed;
    const float radius;

    // Wire interface
public:
    QVector3D deformation(const QVector3D P) const;
};

template <typename Curve,
          float (*Density)(float) = WireDensityLinear>
class VertexWireOneSided : public Wire
{
public:
    VertexWireOneSided(const Curve ref, const Curve dom,
                       const Curve def, const float r,
                       const float delta=DEFAULT_DELTA);
    VertexWireOneSided(const QVector<QVector3D> ref,
                       const QVector<QVector3D> dom,
                       const QVector<QVector3D> def,
                       const float r,
                       const float delta=DEFAULT_DELTA);
// protected:
    const Curve reference;
    const Curve domain;
    const Curve deformed;
    const float radius;
    const float delta;

    // Wire interface
public:
    QVector3D deformation(const QVector3D P) const;
};

}

/* Implementation */

template<typename Curve, float (*Density)(float)>
Wrap::VertexWireRadial<Curve, Density>::VertexWireRadial(
        const Curve ref,
        const Curve def,
        const float r)
    : reference(ref)
    , deformed(def)
    , radius(r)
{}

template<typename Curve, float (*Density)(float)>
Wrap::VertexWireRadial<Curve, Density>::VertexWireRadial(
        const QVector<QVector3D> ref,
        const QVector<QVector3D> def,
        const float r)
    : reference(Curve(ref))
    , deformed(Curve(def))
    , radius(r)
{}

template<typename Curve, float (*Density)(float)>
QVector3D Wrap::VertexWireRadial<Curve, Density>::deformation(const QVector3D P) const
{
    const float t = this->reference.project(P);
    const QVector3D R = this->reference.eval(t);
    const QVector3D TR = this->reference.evalDeriv(t);
    const QVector3D W = this->deformed.eval(t);
    const QVector3D TW = this->deformed.evalDeriv(t);
    const float potential = Density((P - R).length()/radius);
    return wireDeformation(P, R, TR, W, TW, potential);
}

template<typename Curve, float (*Density)(float)>
Wrap::VertexWireOneSided<Curve, Density>::VertexWireOneSided(
        const Curve ref,
        const Curve dom,
        const Curve def,
        const float r,
        const float delta)
    : reference(ref)
    , domain(dom)
    , deformed(def)
    , radius(r)
    , delta(delta)
{}

template<typename Curve, float (*Density)(float)>
Wrap::VertexWireOneSided<Curve, Density>::VertexWireOneSided(
        const QVector<QVector3D> ref,
        const QVector<QVector3D> dom,
        const QVector<QVector3D> def,
        const float r,
        const float delta)
    : VertexWireOneSided(
          Curve(ref)
          , Curve(dom)
          , Curve(def)
          , r
          , delta
          )
{}

template<typename Curve, float (*Density)(float)>
QVector3D Wrap::VertexWireOneSided<Curve, Density>::deformation(const QVector3D P) const
{
    const float tRef = this->reference.project(P);
    const float tDom = this->domain.project(P);
    const QVector3D R = this->reference.eval(tRef);
    const QVector3D TR = this->reference.evalDeriv(tRef);
    const QVector3D D = this->reference.eval(tDom);
    const QVector3D W = this->deformed.eval(tRef);
    const QVector3D TW = this->deformed.evalDeriv(tRef);
    const float PR = (P - R).length();
    const float RD = (D - P).length();
    const float cosangle = QVector3D::dotProduct(D - R, P - R);
    float normalizer = RD;
    if (std::abs(cosangle < delta)) {
        /* Linear Interp(cosangle); refer to the paper */
        normalizer = (1 - cosangle)*radius + cosangle*RD;
    }
    const float potential = Density(PR/normalizer);
    return wireDeformation(P, R, TR, W, TW, potential);
}

#endif // VERTEXWIRE_H
