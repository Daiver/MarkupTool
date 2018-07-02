#ifndef CURVEWRAPPER_ITERATORS_H
#define CURVEWRAPPER_ITERATORS_H

#include <Curves/curves.h>
#include <Curves/impl/curve_unif_iter.h>

namespace Curves {

template<typename Vec>
class CurveSample
                : public CurvesImpl::CurveUniformIteratorBase<
        CurveSample<Vec>,
        Curve<Vec> >
{
public:
    using CurveType = Curve<Vec>;
    using value_type = CurveSample<Vec>;
    using difference_type = int;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::random_access_iterator_tag;
    using VecType = Vec;
    using ImplType = CurvesImpl::CurveUniformIteratorBase<CurveSample<Vec>, Curve<Vec> >;

    using ImplType::CurveUniformIteratorBase;
};

template<typename Vec>
class CurvePointsSample
                : public CurvesImpl::CurveUniformIteratorBase<
        CurvePointsSample<Vec>,
          Curve<Vec>,
          Vec,
          typename Curve<Vec>::ParamType,
          Vec>
{
public:
    using CurveType = Curve<Vec>;
    using value_type = Vec;
    using difference_type = int;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::random_access_iterator_tag;
    using VecType = Vec;
    using ImplType = CurvesImpl::CurveUniformIteratorBase<
        CurvePointsSample<Vec>,
          Curve<Vec>,
          Vec,
          typename Curve<Vec>::ParamType,
          Vec>;

    using ImplType::CurveUniformIteratorBase;

    value_type operator* () const {
       return this->getPoint();
    }
};

template<typename Vec>
class CurveTangentsSample
                : public CurvesImpl::CurveUniformIteratorBase<
        CurveTangentsSample<Vec>,
          Curve<Vec>,
          Vec,
          typename Curve<Vec>::ParamType,
          Vec>
{
public:
    using value_type = Vec;
    using difference_type = int;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::random_access_iterator_tag;
    using VecType = Vec;
    using ImplType = CurvesImpl::CurveUniformIteratorBase<
        CurveTangentsSample<Vec>,
          Curve<Vec>,
          Vec,
          typename Curve<Vec>::ParamType,
          Vec>;

    using ImplType::CurveUniformIteratorBase;

    value_type operator* () const {
       return this->getTangent();
    }
};

template<typename Vec>
CurveSample<Vec> Curve<Vec>::sample(int nPoints) const
{
    return CurveSample<Vec>(this, nPoints);
}
template<typename Vec>
CurveSample<Vec> Curve<Vec>::sample(int nPoints, ParamType from, ParamType to) const
{
    return CurveSample<Vec>(this, nPoints, from, to);
}
template<typename Vec>
CurvePointsSample<Vec> Curve<Vec>::samplePoints(int nPoints) const
{
    return CurvePointsSample<Vec>(this, nPoints);
}
template<typename Vec>
CurvePointsSample<Vec> Curve<Vec>::samplePoints(int nPoints, ParamType from, ParamType to) const
{
    return CurvePointsSample<Vec>(this, nPoints, from, to);
}
template<typename Vec>
CurveTangentsSample<Vec> Curve<Vec>::sampleTangents(int nPoints) const
{
    return CurveTangentsSample<Vec>(this, nPoints);
}
template<typename Vec>
CurveTangentsSample<Vec> Curve<Vec>::sampleTangents(int nPoints, ParamType from, ParamType to) const
{
    return CurveTangentsSample<Vec>(this, nPoints, from, to);
}

}
#endif // CURVEWRAPPER_ITERATORS_H
