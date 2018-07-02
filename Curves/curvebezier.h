#ifndef CURVEWRAPPER_BEZIER_H
#define CURVEWRAPPER_BEZIER_H

#include <Curves/impl/curvebspline_base.h>

namespace Curves {

template<typename Vec>
class CurveBezier
                : public CurvesImpl::CurveBSplineBase<CurveBezier<Vec>, Vec >
{
public:
    using ParamType = typename Curve<Vec>::ParamType;
    using VecType = Vec;
    using CurveType = CurvesMath::BSplinesComb<Vec>;

    /* INHERITED:
     *
     * Param .minParam() const
     * Param .maxParam() const
     * Vec .paramToPoint(Param t) const
     * Vec .paramToTangent(Param t) const
     * Param .pointToParam(Vec P) const
     * QVector<Vec> .getEditpoints() const
     * QVector<Vec> .getCoeffs() const
     * Sample .sample(int nPoints) const;
     * Sample .sample(int nPoints, Param from, Param to) const;
     * Sample .samplePoints(int nPoints) const;
     * Sample .samplePoints(int nPoints, Param from, Param to) const;
     * Sample .sampleTangents(int nPoints) const;
     * Sample .sampleTangents(int nPoints, Param from, Param to) const;
     */

    CurveBezier();
    CurveBezier(QVector<Vec> editpoints);
    virtual void setEditpoints(QVector<VecType> ePts);
};

template<typename Vec>
CurveBezier<Vec>::CurveBezier() { /* empty */ }
template<typename Vec>
CurveBezier<Vec>::CurveBezier(QVector<Vec> editpoints)
{
    setEditpoints(editpoints);
}
template<typename Vec>
void CurveBezier<Vec>::setEditpoints(QVector<Vec> ePts)
{
    Q_ASSERT(ePts.size() == 0 || ePts.size() >= 2);

    const int order = ePts.size() - 1;
    const int nKnots = 2 * (order + 1);
    QVector<ParamType> &knots = this->_knots;
    knots.resize(nKnots);
    for (int i = 0; i <= order; ++i) {
        knots[i] = 0.0;
        knots[knots.size()-i-1] = 1.0;
    }
    this->_order = order;
    this->_coeffs = ePts;
    this->saveEditpoints(ePts);
    this->updateDerivatives();
}
}

#endif // CURVEWRAPPER_BEZIER_H
