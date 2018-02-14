#include "landmark.h"


Landmark::Landmark(qreal x, qreal y, qreal w, qreal h, int indPoint)
    : QGraphicsEllipseItem(0, 0, w, h)
{
    this->setPos(x, y);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    this->setAcceptDrops(true);

    pen = QPen(Qt::red);
    brush = QBrush(Qt::red);
    this->indPoint = indPoint;
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
    rect.setCoords(rect.left()-2, rect.top()-2, rect.right(), rect.bottom());
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(rect);
    //QFont font=painter->font() ;
    //font.setPointSize ( 5);
    //painter->setFont(font);
    //painter->drawText(rect.center(), QString::number(indPoint));
}



void Landmark::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseMoveEvent(event);
    qDebug() << indPoint;
    emit changePosition(event->scenePos());
}

