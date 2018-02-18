#include "landmark.h"

Landmark::Landmark(qreal x, qreal y, qreal w, qreal h, const int ind, QPen pen, QBrush brush) : QGraphicsEllipseItem(0,0,w,h)
{
    this->setPos(x, y);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    this->setAcceptDrops(true);
    this->index = ind;
    this->setRect(-w/2, -h/2, w, h);
    setPen(pen);
    setBrush(brush);
}



void Landmark::setPen(const QPen &pen)
{
    this->pen = pen;
}



void Landmark::setBrush(const QBrush &brush)
{
    this->brush = brush;
}



void Landmark::setSize(const QSize &size)
{
    QPointF pos = this->scenePos();
    this->setRect(pos.x(), pos.y(), size.width(), size.height());
}



void Landmark::setIndex(const int &index)
{
    this->index = index;
}



bool Landmark::getEnd() const
{
    return end;
}



bool Landmark::getStart() const
{
    return start;
}



bool Landmark::getUp() const
{
    return up;
}



void Landmark::setEnd(const bool value)
{
    end = value;
}



void Landmark::setStart(const bool value)
{
    start = value;
}



void Landmark::setUp(const bool value)
{
    up = value;
}



void Landmark::setSize(const double &size)
{
    this->setRect(-size/2, -size/2, size, size);
}



void Landmark::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    if (end || !up){
        pen.setColor(Qt::blue);
        brush.setColor(Qt::blue);
    }

    if (start || up){
        pen.setColor(Qt::red);
        brush.setColor(Qt::red);
    }

    QRectF rect = this->boundingRect();
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(rect);

}



void Landmark::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseMoveEvent(event);
    qDebug() << index;
    emit changePosition(event->scenePos());
}
