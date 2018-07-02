#ifndef CURVE_UNIF_ITER_H
#define CURVE_UNIF_ITER_H

#include <iterator>

namespace CurvesImpl {

template<typename Final,
           typename Curve=typename Final::CurveType,
           typename Vec=typename Curve::VecType,
           typename Param=typename Curve::ParamType,
           typename ValueType=Final
         >
class CurveUniformIteratorBase
{
public:
    using iterator_type = Final;
    using difference_type = int;
    using value_type = ValueType;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::random_access_iterator_tag;
    using VecType = Vec;
    using ParamType = Param;

    // CurveUniformIteratorBase()
    //     : t0(0), t1(0), maxIdx(0), curve(nullptr)
    // {}
    CurveUniformIteratorBase(const Curve *curve,
                           int nPoints,
                           ParamType t0,
                           ParamType t1);

    CurveUniformIteratorBase(const Curve *curve,
                            int nPoints);

    inline ParamType getParam() const
    {
        ParamType n = maxIdx;
        ParamType i = curIdx;
        return (n - i)/n*t0 + i/n*t1;
    }

    inline Vec getPoint() const { return curve->paramToPoint(getParam()); }
    inline Vec getTangent() const { return curve->paramToTangent(getParam()); }
    /* hope compiler will know when to ignore it */
    inline const value_type& operator* () const { return *reinterpret_cast<const Final*>(this); }

    inline const iterator_type &operator++()
    {
        Q_ASSERT (curIdx <= maxIdx);
        curIdx += 1;
        return *reinterpret_cast<Final*>(this);
    }

    inline difference_type operator-(const iterator_type &rhs)
    { return curIdx - rhs.curIdx; }

    inline bool operator!=(const iterator_type &rhs)
    { return curIdx != rhs.curIdx; }

    inline bool operator==(const iterator_type &rhs) const
    { return curIdx == rhs.curIdx; }

    inline iterator_type begin() const
    {
        iterator_type it = *reinterpret_cast<const Final*>(this);
        it.curIdx = 0;
        return it;
    }

    inline iterator_type end() const
    {
        /* kinda slow for this kind of an operation */
        iterator_type it = *reinterpret_cast<const Final*>(this);
        it.curIdx = it.maxIdx + 1;
        return it;
    }

    inline QVector<value_type> toQVector() const
    {
        const Final *self = reinterpret_cast<const Final *>(this);
        QVector<value_type> vec;
        vec.reserve(self->end() - self-> begin());
        for (value_type v: *self) {
            vec.append(v);
        }
        return vec;
    }

protected:
    const Curve *curve;
    const Param t0;
    const Param t1;
    const Param maxIdx;
    int curIdx;
};

template<typename Final,
           typename Curve,
           typename Vec,
           typename Param,
           typename ValueType>
CurveUniformIteratorBase<Final, Curve, Vec, Param, ValueType>::CurveUniformIteratorBase(
        const Curve *curve,
          int nPoints,
          Param t0,
          Param t1)
    : curve(curve),
      curIdx(0),
      maxIdx(nPoints - 1),
      t0(t0),
      t1(t1)
{ /* empty */ }

template<typename Final,
           typename Curve,
           typename Vec,
           typename Param,
           typename ValueType>
CurveUniformIteratorBase<Final, Curve, Vec, Param, ValueType>::CurveUniformIteratorBase(
        const Curve *curve,
          int nPoints)
    : curve(curve),
      curIdx(0),
      maxIdx(nPoints - 1),
      t0(curve->minParam()),
      t1(curve->maxParam())
{ /* empty */ }

template<typename Curve,
           typename Vec=typename Curve::VecType,
           typename Param=typename Curve::ParamType>
class CurveUniformIterator
        : public CurveUniformIteratorBase<CurveUniformIterator<Curve, Vec, Param>, Curve >
{
public:
    using crtp_base = CurveUniformIteratorBase<CurveUniformIterator<Curve, Vec, Param>, Curve >;
    using crtp_base::CurveUniformIteratorBase;
};

}

#endif // CURVE_UNIF_ITER_H
