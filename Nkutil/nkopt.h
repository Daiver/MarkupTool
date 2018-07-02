#ifndef NKOPT_H
#define NKOPT_H

#include <cmath>
#include "Nkutil/nkutil.h"

class OptimizationState
{
using Numeric = double;
public:
    OptimizationState(
            Numeric lr=.02,
              int nIters=2048,
              Numeric eps=1e-5,
              Numeric movAvgLag=.75)
        : obj(std::numeric_limits<Numeric>::infinity()),
          updateQty(std::numeric_limits<Numeric>::infinity()),
          avgObjChange(std::numeric_limits<Numeric>::infinity()),
          avgUpdateQty(std::numeric_limits<Numeric>::infinity()),
          it(0),
          maxIter(nIters - 1),
          eps(eps),
          lr(lr),
          lag(movAvgLag)
    { /* empty */ }

    inline bool done() const
    {
        return it > maxIter
                || std::abs(lr) < eps
                || converged();
    }
    inline bool converged() const
    {
        return  std::abs(obj) < eps
                || avgUpdateQty < eps
                || avgObjChange < eps;
    }
    inline Numeric objective() const { return obj; }
    inline Numeric learningRate() const { return lr; }
    inline Numeric threshold() const { return eps; }
    inline int iter() const { return it; }

    inline void setObjective(Numeric newObj)
    {
        if (std::isinf(avgObjChange) || std::isnan(avgObjChange)) {
            avgObjChange = 1.0;
        }
        avgObjChange = lag*avgObjChange + (1.0 - lag)*(newObj / obj);
        obj = newObj;
    }

    inline void setUpdateQty(Numeric newQty)
    {
        newQty = std::abs(newQty);
        if (std::isinf(avgUpdateQty) || std::isnan(avgUpdateQty)) {
            avgUpdateQty = newQty;
        }
        avgUpdateQty = lag*avgUpdateQty + (1.0-lag)*newQty;
        updateQty = newQty;
    }

    inline void finishIteration()
    {
            it = std::min(maxIter + 1, it + 1);

            /* yet to do; also may be useless for in that case
             * we'd need to rollback a few iterations
             */
            const bool tooBigUpdate = false;
             /* the constants here are chosen arbitrary */
            const bool overshooting = avgObjChange >= 1.0 + 5.0*eps;

            if (tooBigUpdate || overshooting) {
                    lr *= 0.75;
            }


    }

private:
    Numeric obj;
    Numeric avgObjChange;
    Numeric updateQty;
    Numeric avgUpdateQty;
    Numeric lr;
    int it;
    const int maxIter;
    const Numeric eps;
    const Numeric lag;
};

#endif // NKOPT_H
