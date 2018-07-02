#ifndef CURVEWRAPPER_CATMULLROM_H
#define CURVEWRAPPER_CATMULLROM_H

#include "Curves/math/catmullrom.h"
#include "Curves/impl/curve_proxy.h"

namespace Curves {

template<typename Vec>
class CurveCatmullRom
                : public CurvesImpl::CurveProxy<CurveCatmullRom<Vec>,
                                          CurvesMath::CatmullRom<Vec>,
                                          Vec >
{
public:
    using ParamType = typename Curve<Vec>::ParamType;
    using VecType = Vec;
    using CurveType = CurvesMath::CatmullRom<Vec>;


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

    CurveCatmullRom(QVector<Vec> editpoints);
    virtual ParamType pointToParam(VecType P) const;
    virtual QVector<VecType> editpoints() const;
    virtual void setEditpoints(QVector<VecType> ePts);
    inline CurvesMath::CatmullRom<Vec> impl() const;
    inline QVector<Vec> controlPoints() const;

private:
    QVector<Vec> _editpoints;
    QVector<Vec> _coeffs;
};

template<typename Vec>
CurveCatmullRom<Vec>::CurveCatmullRom(QVector<Vec> editpoints)
{
    setEditpoints(_editpoints);
}
template<typename Vec>
typename CurveCatmullRom<Vec>::ParamType
CurveCatmullRom<Vec>::pointToParam(VecType P) const
{
    return this->impl().pointToParam(P);
}

template<typename Vec>
QVector<Vec> CurveCatmullRom<Vec>::editpoints() const
{
    return this->_editpoints;
}

template<typename Vec>
void CurveCatmullRom<Vec>::setEditpoints(QVector<Vec> ePts)
{
    Q_ASSERT(ePts.size() == 0 || ePts.size() >= 2);

    this->_editpoints = ePts;
    const int nNewCoeffs = ePts.size() > 0 ? ePts.size() + 2 : 0;
    QVector<VecType> &newCoeffs = this->_coeffs;
    newCoeffs.resize(nNewCoeffs);
    if (ePts.size() > 0) {
        newCoeffs.first() = ePts.first();
        newCoeffs.last() = ePts.last();
        std::copy(ePts.begin(),
                  ePts.end(),
                  newCoeffs.begin() + 1);
    }
    this->_coeffs = newCoeffs;
}

template<typename Vec>
inline CurvesMath::CatmullRom<Vec> CurveCatmullRom<Vec>::impl() const
{
    return CurvesMath::CatmullRom<Vec>(_coeffs);
}

template<typename Vec>
inline QVector<Vec> CurveCatmullRom<Vec>::controlPoints() const
{
    return this->_coeffs;
}

}

#endif // CURVEWRAPPER_CATMULLROM_H
