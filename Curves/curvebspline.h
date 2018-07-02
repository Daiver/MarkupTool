#ifndef CURVEWRAPPER_BSPLINES_H
#define CURVEWRAPPER_BSPLINES_H

#include <Curves/impl/curvebspline_base.h>
#include <Curves/curves.h>

namespace Curves {

template<typename Vec>
class CurveBSpline
                : public CurvesImpl::CurveBSplineBase<CurveBSpline<Vec>, Vec >
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
     * QVector<Vec> .controlPoints() const
     * Sample .sample(int nPoints) const;
     * Sample .sample(int nPoints, Param from, Param to) const;
     * Sample .samplePoints(int nPoints) const;
     * Sample .samplePoints(int nPoints, Param from, Param to) const;
     * Sample .sampleTangents(int nPoints) const;
     * Sample .sampleTangents(int nPoints, Param from, Param to) const;
     */

    CurveBSpline(QVector<Vec> editpoints, int _order=3);

    virtual void setEditpoints(QVector<VecType> ePts);
    virtual void setCoeffs(QVector<VecType> _coeffs);
};




/* Implementation */

template<typename Vec>
CurveBSpline<Vec>::CurveBSpline(QVector<Vec> editpoints, int order)
{
    this->_order = order;
    setEditpoints(editpoints);
}

template<typename Vec>
void CurveBSpline<Vec>::setEditpoints(QVector<VecType> ePts)
{
    Q_ASSERT(ePts.size() == 0 || ePts.size() >= 2);

    CurvesMath::BSplinesComb<Vec>::interpolatePoints(static_cast<int>(this->_order),
                                                     static_cast<const QVector<Vec> &>(ePts),
                                                     /* output: */
                                                     this->_editparams,
                                                     this->_knots,
                                                     this->_coeffs);
    this->updateDerivatives();
}
template<typename Vec>
void CurveBSpline<Vec>::setCoeffs(QVector<VecType> coeffs)
{
    Q_ASSERT(coeffs.size() == this->_coeffs.size()); /* not ready to allow otherwise */
    this->_coeffs = coeffs;
}

}

#endif // CURVEWRAPPER_BSPLINES_H
