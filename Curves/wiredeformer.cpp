#include "wiredeformer.h"

namespace Wrap {

DefaultBlender::DefaultBlender(const float m,
                               const float eps) :
    m(m),
    eps(eps),
    accMagnitude(0)
{}

void DefaultBlender::reset()
{
    this->accMagnitude = 0;
    this->accVec = QVector3D();
}

void DefaultBlender::add(QVector3D def)
{
    const float norm = def.length();
    if (norm < eps) {
        return;
    }
    const float weight = std::pow(norm, m);
    accMagnitude += weight;
    accVec += weight*def;
}

QVector3D DefaultBlender::result() const
{
    if (accMagnitude >= this->eps)
        return accVec/accMagnitude;
    else
        return QVector3D();
}

template <typename Blender>
void wireDeform(
        const QVector<QVector3D> points,
        const QVector<Wire*> wires,
        Blender blender,
        QVector<QVector3D> &out)
{
    out.resize(points.size());
    for (int i = 0; i < points.size(); ++i) {
        blender.reset();
        const QVector3D P = points[i];
        for (auto wIt = wires.begin(); wIt != wires.end(); ++wIt) {
            const QVector3D defPart = (**wIt).deformation(P);
            blender.add(defPart);
        }
        out[i] = P + blender.result();
    }
}

QVector<QVector3D> wireDeform(
        const QVector<QVector3D> points,
        const QVector<Wire*> wires,
        const float m) /* exponent for the blender */
{
    QVector<QVector3D> out;
    out.resize(points.size());
    wireDeform(points, wires, DefaultBlender(m), out);
    return out;
}

}
