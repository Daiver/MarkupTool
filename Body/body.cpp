#include "body.h"

void Part::addPoint(Landmark *point, int indInsert)
{
    points.insert(indInsert, point);

    if (points.size() == 4){
        points[2]->setEnd(true);
        points[0]->setStart(true);
    }

    if (indInsert == 0 || indInsert == 1 || indInsert == 2)
        point->setUp(true);

    for (int indPoint = indInsert; indPoint < points.size();indPoint++){
        if (points[indPoint]->getEnd() == true && indInsert < indPoint)
            point->setUp(true);
        points[indPoint]->setIndex(indPoint);
    }

    update();
}



void Part::loadPoint(Landmark *point, int indInsert)
{
    points.insert(indInsert, point);
    point->setUp(true);

    for (int indPoint = 0; indPoint < points.size(); indPoint++)
        if (points[indPoint]->getEnd() == true && indInsert > indPoint)
            point->setUp(false);

    for (int indPoint = indInsert; indPoint < points.size();indPoint++)
        points[indPoint]->setIndex(indPoint);

    update();
}



void Part::deletePoint(int indPoint)
{
    points.removeAt(indPoint);
    update();
}



void Part::update()
{
    corner.clear();
    up.clear();
    down.clear();

    for (int indPoint = 0; indPoint < points.size(); indPoint++){
        if (points[indPoint]->getStart() == true || points[indPoint]->getEnd() == true)
            corner.push_back(points[indPoint]->scenePos());
        if (points[indPoint]->getUp() == true && points[indPoint]->getStart() == false && points[indPoint]->getEnd() == false)
            up.push_back(points[indPoint]->scenePos());
        if (points[indPoint]->getUp() == false)
            down.push_back(points[indPoint]->scenePos());
    }
}



Landmark *Part::getEnd()
{
    for (int indPoint = 0; indPoint < pointsSize(); indPoint++)
        if (points[indPoint]->getEnd() == true)
            return points[indPoint];

    return nullptr;
}



Landmark *Part::getStart()
{
    for (int indPoint = 0; indPoint < pointsSize(); indPoint++)
        if (points[indPoint]->getStart() == true)
            return points[indPoint];

    return nullptr;
}



QPointF Part::getCentral()
{
    QPointF start = getStart()->scenePos();
    QPointF end = getEnd()->scenePos();

    QPointF central = (end+start)/2;
    return central;
}



QRect Part::getBox() const
{
    float xMin = 100000000, yMin = 100000000, xMax = 0, yMax = 0;

    for (int indPoint = 0; indPoint < pointsSize(); indPoint++){
        float x = points[indPoint]->scenePos().x();
        float y = points[indPoint]->scenePos().y();

        if (x > xMax)
            xMax = x;
        if (x < xMin)
            xMin = x;
        if (y > yMax)
            yMax = y;
        if (y < yMin)
            yMin = y;
    }

    float w = xMax - xMin;
    float h = yMax - yMin;
    float x = xMin;
    float y = yMin;
    return QRect(x, y, w, h);
}



int Part::pointsSize() const
{
    return points.size();
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



Part *Body::getActivedPart()
{
    return &parts[indActived];
}



bool Body::isEmpty() const
{
    for (int indPart = 0; indPart < parts.size(); indPart++)
        if (parts[indPart].pointsSize() != 0)
            return false;

    return true;
}
