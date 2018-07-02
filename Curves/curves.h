#ifndef CURVES_H
#define CURVES_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include <Nkutil/nkutil.h>
#include <Nkutil/curves.h>
#include "Curves/math/bspline.h"

namespace Curves {

typedef QVector2D Vec2;
typedef QVector3D Vec3;


template<typename Vec>
class CurveSample;

template<typename Vec>
class CurvePointsSample;

template<typename Vec>
class CurveTangentsSample;

/* I'm giving it up,
 * no attempts to make stack-allocatable structure,
 * no attempts to split changeable parts into policies,
 *
 * just plain&slow **vtable.**
 *
 * Actually, even two vtables,
 * s.t. I don't have to remove the little code
 * which allocate Curve on stack.
 *
 * Sure policies and mixins would've been better.
 *
 * Also I'm still using Curve as a namespace
 * for helpers like .createBernstein()
 */

template<typename Vec=Vec3>
class Curve; /* Abstract class for vtable */

/* The interface definition */

template<typename Vec>
class Curve
{
public:
    using VecType = Vec;
    using ParamType = float;

    /* The `Curve` is a parameterized curve
     * i.e. a smooth mapping from an interval of a real line into vector space;
     * it's parameterization interval is:
     *                  $$ (.minParam(), .maxParam()). $$
     *
     * A `ParamType` is a number from that range.
     * A parameter can be consider a local coordinate of a point on the curve;
     */
    virtual ParamType minParam() const = 0;
    virtual ParamType maxParam() const = 0;

    virtual VecType paramToPoint(ParamType t) const = 0;
    virtual VecType paramToTangent(ParamType t) const = 0;
    virtual ParamType pointToParam(VecType P) const = 0; /* point projection */

    virtual QVector<VecType> editpoints() const = 0;
    /* a candidate for removal (or a an extended "EditableCurve" interface) */
    virtual void setEditpoints(QVector<VecType> ePts) = 0;

    virtual CurveSample<VecType> sample(int nPoints) const;
    virtual CurveSample<VecType> sample(int nPoints, ParamType from, ParamType to) const;
    virtual CurvePointsSample<VecType> samplePoints(int nPoints) const;
    virtual CurvePointsSample<VecType> samplePoints(int nPoints, ParamType from, ParamType to) const;
    virtual CurveTangentsSample<VecType> sampleTangents(int nPoints) const;
    virtual CurveTangentsSample<VecType> sampleTangents(int nPoints, ParamType from, ParamType to) const;
};

}

#include "Curves/impl/curve_samples.h"

#endif // CURVES_H
