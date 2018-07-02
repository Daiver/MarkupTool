#ifndef WIRECATMULLROM_H
#define WIRECATMULLROM_H

#include "catmullrom.h"
#include "vertexwire.h"
#include "wiredeformer.h"

namespace Wrap {

class WireCatmullRomRadial
        : public VertexWireRadial<CatmullRomCurve<QVector3D> >
{
    using VertexWireRadial<CatmullRomCurve<QVector3D>, WireDensityLinear>
    ::VertexWireRadial;
};

class WireCatmullRomOneSided
        : public VertexWireOneSided<CatmullRomCurve<QVector3D> >
{
    using VertexWireOneSided<CatmullRomCurve<QVector3D>, WireDensityLinear>
    ::VertexWireOneSided;
};

}

#endif // WIRECATMULLROM_H
