#ifndef CURVEBSPLINE_BASE_H
#define CURVEBSPLINE_BASE_H

#include <Curves/curves.h>
#include <Curves/math/bspline.h>
#include <Curves/impl/curve_proxy.h>

namespace CurvesImpl {

using namespace ::Curves;

template<typename Final,
           typename Vec>
class CurveBSplineBase
        : public CurvesImpl::CurveProxy<Final, CurvesMath::BSplinesComb<Vec> >
{
public:
    using ParamType = typename Curve<Vec>::ParamType;
    using VecType = Vec;
    using CurveType = CurvesMath::BSplinesComb<Vec>;

    virtual QVector<VecType> editpoints() const
    {
        QVector<VecType> ePts(_editparams.size());
        for (int i = 0; i < _editparams.size(); ++i) {
            ePts[i] = this->paramToPoint(_editparams[i]);
        }
        return ePts;
    }

    /* INHERITED:
     *
     * Param .minParam() const
     * Param .maxParam() const
     * Vec .paramToPoint(Param t) const
     * Vec .paramToTangent(Param t) const
     * Param .pointToParam(Vec P) const
     * QVector<Vec> .controlPoints() const
     * Sample .sample(int nPoints) const;
     * Sample .sample(int nPoints, Param from, Param to) const;
     * Sample .samplePoints(int nPoints) const;
     * Sample .samplePoints(int nPoints, Param from, Param to) const;
     * Sample .sampleTangents(int nPoints) const;
     * Sample .sampleTangents(int nPoints, Param from, Param to) const;
     */

    virtual QVector<VecType> controlPoints() const
    {
        return this->_coeffs;
    }

    /* no setControlPoints() here as it is now always allowed
     * to set them aribtrarily
     */

    inline CurvesMath::BSplinesComb<Vec> impl() const
    {
        return CurvesMath::BSplinesComb<Vec>(_order, _knots, _coeffs);
    }

    virtual ParamType pointToParam(VecType P) const
    {
        CurvesMath::BSplinesCombInverse<Vec> inv(_order, _knots,
                                                 _coeffs, _coeffsD, _coeffsD2);
        return inv.pointToParam(P);
    }

protected:
    int _order;
    QVector<Vec> _coeffs;
    QVector<Vec> _coeffsD;
    QVector<Vec> _coeffsD2;
    QVector<ParamType> _knots;
    QVector<ParamType> _editparams;

    void saveEditpoints(QVector<VecType> ePts)
    {
        this->_editparams.resize(ePts.size());
        for (int i = 0; i < ePts.size(); ++i) {
            this->_editparams[i] = this->pointToParam(ePts[i]);
        }
    }

    void updateDerivatives()
    {
        auto dc = CurvesMath::BSplinesComb<Vec>(this->_order,
                                                this->_knots,
                                                this->_coeffs).derivativeFunction();
        auto d2c = dc.derivativeFunction();
        this->_coeffsD = dc.controlPoints();
        this->_coeffsD2 = d2c.controlPoints();
    }
};

}

#endif // CURVEBSPLINE_BASE_H
