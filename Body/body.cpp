#include "body.h"

void Segment::addPoint(Landmark *point, int indInsert)
{
    for (int indSegment = 0; indSegment < invisibleSegments.size(); indSegment++){
        for (int indPoint = 0; indPoint < invisibleSegments[indSegment].size(); indPoint++){
            if (indInsert <= invisibleSegments[indSegment][indPoint]){
               invisibleSegments[indSegment][indPoint] =  invisibleSegments[indSegment][indPoint] + 1;
            }
        }
    }

    points.insert(indInsert, point);

    if (points.size() == 4){
        points[2]->setEnd(true);
        points[0]->setStart(true);
        points[1]->setUpCentral(true);
        points[3]->setDownCentral(true);
    }

    if (indInsert == 0 || indInsert == 1 || indInsert == 2)
        point->setUp(true);

    for (int indPoint = indInsert; indPoint < points.size();indPoint++){
        if (points[indPoint]->isEnd() == true && indInsert < indPoint)
            point->setUp(true);
        points[indPoint]->setIndex(indPoint);
    }
    update();
}



void Segment::loadPoint(Landmark *point, int indInsert)
{
    points.insert(indInsert, point);
    point->setUp(true);

    for (int indPoint = 0; indPoint < points.size(); indPoint++)
        if (points[indPoint]->isEnd() == true && indInsert > indPoint)
            point->setUp(false);

    for (int indPoint = indInsert; indPoint < points.size();indPoint++)
        points[indPoint]->setIndex(indPoint);

    update();
}



void Segment::setInvisible(int indPoint, bool value)
{
    points[indPoint]->setInvsibile(value);
    invisible.push_back(indPoint);
}



void Segment::setInvisibleSegment(int indPoint, bool value)
{
    if (invisibleSegments.size() == 0)
        invisibleSegments.push_back(QVector<int>());

    points[indPoint]->setInvsibile(value);
    invisible.push_back(indPoint);

    if (value == false && invisibleSegments[indInvisibleSegment].contains(indPoint))
        invisibleSegments[indInvisibleSegment].removeOne(indPoint);
    else
        invisibleSegments[indInvisibleSegment].push_back(indPoint);
}



void Segment::clearInvisibleSegment()
{
    invisibleSegments.clear();
}



void Segment::deletePoint(int indPoint)
{
    points.removeAt(indPoint);
    update();
}



void Segment::update()
{
    invisible.clear();
    corner.clear();
    centers.clear();
    up.clear();
    down.clear();

    for (int indPoint = 0; indPoint < points.size(); indPoint++){
        if (points[indPoint]->isInvisble() == true)
            invisible.push_back(indPoint);
        if (points[indPoint]->isStart() == true || points[indPoint]->isEnd() == true)
            corner.push_back(points[indPoint]->scenePos());
        if (points[indPoint]->isUpCentral() == true || points[indPoint]->isDownCentral() == true)
            centers.push_back(points[indPoint]->scenePos());
        if (points[indPoint]->isUp() == true && points[indPoint]->isStart() == false && points[indPoint]->isEnd() == false)
            up.push_back(points[indPoint]->scenePos());
        if (points[indPoint]->isUp() == false)
            down.push_back(points[indPoint]->scenePos());
    }
}



Landmark *Segment::getEnd()
{
    for (int indPoint = 0; indPoint < getPointsSize(); indPoint++)
        if (points[indPoint]->isEnd() == true)
            return points[indPoint];

    return nullptr;
}



Landmark *Segment::getStart()
{
    for (int indPoint = 0; indPoint < getPointsSize(); indPoint++)
        if (points[indPoint]->isStart() == true)
            return points[indPoint];

    return nullptr;
}



QPointF Segment::getCentral()
{
    QPointF start = getStart()->scenePos();
    QPointF end = getEnd()->scenePos();

    QPointF central = (end+start)/2;
    return central;
}



QRect Segment::getBox() const
{
    float xMin = 100000000, yMin = 100000000, xMax = 0, yMax = 0;

    for (int indPoint = 0; indPoint < getPointsSize(); indPoint++){
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



int Segment::getPointsSize() const
{
    return points.size();
}



void Segment::nextInvisibleSegment()
{
    if (invisibleSegments.size() <= indInvisibleSegment+1)
        invisibleSegments.push_back(QVector<int>());

    if (invisibleSegments[indInvisibleSegment].size() == 0){
        invisibleSegments.removeAt(indInvisibleSegment);
        indInvisibleSegment--;
    }

    indInvisibleSegment++;
}



void Segment::pastInvisibleSegment()
{
    if (indInvisibleSegment-1 < 0)
        return;

    if (invisibleSegments[indInvisibleSegment].size() == 0)
        invisibleSegments.removeAt(indInvisibleSegment);

    indInvisibleSegment--;
}



ShapeFace::ShapeFace()
{
    Segment leftEye;
    Segment rightEye;
    Segment mouth;

    parts.push_back(leftEye);
    parts.push_back(rightEye);
    parts.push_back(mouth);
}



Segment *ShapeFace::getActivedPart()
{
    return &parts[indActived];
}



bool ShapeFace::isEmpty() const
{
    for (int indPart = 0; indPart < parts.size(); indPart++)
        if (parts[indPart].getPointsSize() != 0)
            return false;

    return true;
}
