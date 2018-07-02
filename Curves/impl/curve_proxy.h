#ifndef CURVEWRAPPER_PROXY_BASE_H
#define CURVEWRAPPER_PROXY_BASE_H

#include <Curves/curves.h>

namespace CurvesImpl {

template<typename Final,
           typename Impl,
           typename Vec=typename Impl::VecType
         >
class CurveProxy
        : public Curves::Curve<Vec>
{
public:
    using CurveType = Impl;
    using ParamType = typename Curves::Curve<Vec>::ParamType;
    using VecType = Vec;

    virtual ParamType minParam() const
    {
        auto self = reinterpret_cast<const Final *>(this);
        return self->impl().minParam();
    }
    virtual ParamType maxParam() const
    {
        auto self = reinterpret_cast<const Final *>(this);
        return self->impl().maxParam();
    }
    virtual VecType paramToPoint(ParamType t) const
    {
        auto self = reinterpret_cast<const Final *>(this);
        return self->impl().paramToPoint(t);
    }
    virtual VecType paramToTangent(ParamType t) const
    {
        auto self = reinterpret_cast<const Final *>(this);
        return self->impl().paramToTangent(t);
    }
    virtual ParamType pointToParam(VecType P) const
    {
        auto self = reinterpret_cast<const Final *>(this);
        return self->impl().pointToParam(P);
    }

    QVector<Vec> getCoeffs() const
    {
        auto self = reinterpret_cast<const Final *>(this);
        return self->impl().getCoeffs();
    }

    /* TODO: implement iterators that use Impl directly,
     * not through ACurve<Vec> and vtable
     */
    using Curves::Curve<Vec>::sample;
    using Curves::Curve<Vec>::samplePoints;
    using Curves::Curve<Vec>::sampleTangents;

protected:
    CurveProxy() {}
};

}

#endif // CURVEWRAPPER_PROXY_BASE_H
