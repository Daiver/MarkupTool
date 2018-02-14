#include "body.h"


void Part::addPoint(Landmark *point, int indInsert)
{
    points.insert(indInsert, point);

    if (points.size() == 4){
        points[2]->end = true;
        points[0]->start = true;
    }
    if (indInsert == 0 || indInsert == 1 || indInsert == 2)
        point->up = true;

    for (int indPoint = indInsert; indPoint < points.size();indPoint++){
        if (points[indPoint]->end == true && indInsert < indPoint)
            point->up = true;
        points[indPoint]->indPoint = indPoint;
    }
}


void Part::loadPoint(Landmark *point, int indInsert)
{
    points.insert(indInsert, point);
    point->up = true;

    for (int indPoint = 0; indPoint < points.size(); indPoint++)
        if (points[indPoint]->end == true && indInsert > indPoint)
            point->up = false;

    for (int indPoint = indInsert; indPoint < points.size();indPoint++)
        points[indPoint]->indPoint = indPoint;
}


Body::Body()
{
    Part leftEye;
    Part rightEye;
    Part mouth;

    parts.push_back(leftEye);
    parts.push_back(rightEye);
    parts.push_back(mouth);
}
