#ifndef CURVEINVERSE_BASE_H
#define CURVEINVERSE_BASE_H

#include <cmath>
#include <QDebug>
#include <Nkutil/nkutil.h>

namespace CurvesImpl {

template<typename FinalInverter, typename Vec, typename Param>
class CurveInverseBase
{
public:

    /* Assuming
     * FinalInverter::c(t)
     * FinalInverter::dc(t)
     * FinalInverter::d2c(t)
     * FinalInverter::minUpdate()
     * exist
     */

    Param pointToParam(Vec P, Param t0, Param t1) const;
    float objective(Vec C, Vec P) const;
    float dObjective(Vec C, Vec dC, Vec P) const; /* derivative */
    float d2Objective(Vec C,
                      Vec dC,
                      Vec d2C,
                      Vec P) const; /* 2nd derivative */
    /* whoa! this way I don't have to have constructors in the crtp base! */
    Param minUpdate() const;
    Param objectiveTolerance() const;
    Param infimumObjective() const;
};

template<typename FinalInverter, typename Vec, typename Param>
inline Param CurveInverseBase<FinalInverter, Vec, Param>::minUpdate() const
{
    return 1e-10;
}

template<typename FinalInverter, typename Vec, typename Param>
inline Param CurveInverseBase<FinalInverter, Vec, Param>::objectiveTolerance() const
{
    return 1e-8*1e-8;
}

template<typename FinalInverter, typename Vec, typename Param>
inline Param CurveInverseBase<FinalInverter, Vec, Param>::infimumObjective() const
{
    return 0.0; /* min distance */
}

template<typename FinalInverter, typename Vec, typename Param>
inline float CurveInverseBase<FinalInverter, Vec, Param>::objective(Vec C,
                                                                    Vec P) const
{
    const auto trueObjective = .5*Vec::dotProduct(C - 2*P, C);
    const auto distance = (C - P).lengthSquared();
    return distance;
}

template<typename FinalInverter, typename Vec, typename Param>
inline float CurveInverseBase<FinalInverter, Vec, Param>::dObjective(Vec C,
                                                                     Vec dC,
                                                                     Vec P) const
{
    return Vec::dotProduct(dC, C - P);
}

template<typename FinalInverter, typename Vec, typename Param>
inline float CurveInverseBase<FinalInverter, Vec, Param>::d2Objective(Vec C,
                                                                      Vec dC,
                                                                      Vec d2C,
                                                                      Vec P) const
{
    return Vec::dotProduct(d2C, C - P) + dC.lengthSquared();
}

template<typename FinalInverter, typename Vec, typename Param>
Param CurveInverseBase<FinalInverter, Vec, Param>::pointToParam(Vec P,
                                                                Param t0,
                                                                Param t1) const
{
    const FinalInverter *self = static_cast<const FinalInverter*>(this);
    Param argmin = t0;
    float min = self->objective(self->c(argmin), P);

    /* objective at the right end
     * The objective is $J = \frac12 \langle C(t) - 2P, C(t) \rangle$
     *
     * We'll use Newton's iteration to find the $t$
     * that achieves $DJ(t) = 0$.
     */
    const float obj1 = self->objective(self->c(t1), P);
    if (obj1 <= min) {
        argmin = t1;
        min = obj1;
    }

    Param t = .5 * (t0 + t1);
    /* Newton's supposed to converge
     * in just a few steps for such a good function */
    int optStepsLimit = 100;
    int nOuts = 0; /* # of times we jumped off the grid */
    const int maxNOuts = 2;
    bool smallDiff = false;
    bool minimumUpdated = false;
    bool smallError = false;
    bool done = false;
    while (!done) {
        t = std::max(t0, std::min(t, t1));
        done = minimumUpdated && (smallDiff || smallError);
        done |= optStepsLimit <= 0;
        optStepsLimit--;

        const Vec C = self->c(t);
        const float obj = self->objective(C, P);
        if (obj <= min) {
            argmin = t;
            min = obj;
        }
        minimumUpdated = true;
        smallError = std::abs(obj - self->infimumObjective()) < self->objectiveTolerance();

        if ((nOuts > maxNOuts) || smallDiff || smallError)
            continue;

        const Vec dC = self->dc(t);
        const float dObj = self->dObjective(C, dC, P);
        const Vec d2C = self->d2c(t);
        const float d2Obj = self->d2Objective(C, dC, d2C, P);
        /*
         * qDebug() << qPrintable(QString("c=(%1,%2);"
         *                                " dc=(%2, %3);"
         *                                " d2c=(%4,%5);"
         *                                " obj=%6;"
         *                                " dobj=%7;"
         *                                " d2obj=%8;"
         *                                " t=%9;"
         *                                " argmin=%10;"
         *                                " min=%11;")
         *                        .arg(C.x()).arg(C.y())
         *                        .arg(dC.x()).arg(dC.y())
         *                        .arg(d2C.x()).arg(d2C.y())
         *                        .arg(obj).arg(dObj).arg(d2Obj)
         *                        .arg(t).arg(argmin).arg(min));
         */
        if (std::abs(dObj) < nkutil::EPS) {
            smallDiff = true;
            continue;
        }
        if(!(std::abs(d2Obj) >= nkutil::EPS)) {
            qDebug() << "CurveInverseBase::closest(): d2Obj is too small";
            /* TODO: try and add grdescent step here as a fallback */
        } else {
            const Param dt =  - dObj/d2Obj;
            Q_ASSERT(!std::isnan(dt));
            t = t + dt;
            smallDiff = /* std::abs(dObj) < eps && */ std::abs(dt) < self->minUpdate();
        }
        if (t <= t0 || t1 <= t) {
            nOuts += 1;
        }
        t = std::max(t0, std::min(t, t1));
        minimumUpdated = false;
    }
    return argmin;
}

}

#endif // CURVEINVERSE_BASE_H
