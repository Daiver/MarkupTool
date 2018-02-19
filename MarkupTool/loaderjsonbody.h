#ifndef LOADERJSONBODY_H
#define LOADERJSONBODY_H

#include "loaderjson.h"
#include "body.h"

namespace LoaderJSON
{
    Body getBody(const QJsonDocument &doc);
    QJsonDocument createJson(const Body *body);
}

#endif // LOADERJSONBODY_H
