#ifndef CURVE_FIT_H
#define CURVE_FIT_H

template<typename Curve, typename Vec=typename Curve::VecType>
class UniformParamAllocation
{
public:
    typedef Curve CurveType;

    UniformParamAllocation(const CurveType &c, QVector<Vec> points)
        : t0(c.minParam()),
          t1(c.maxParam()),
          n(points.size()),
          i(0)
    { /* empty */ }

    bool end() const { return i >= n; }
    void next() { if(!end()) i += 1; }
    typename CurveType::ParamType get() { return (n - i)/n*t0 + i/n*t1; }

private:
    const float t0;
    const float t1;
    const float n;
    float i;
};

#endif // CURVE_FIT_H
