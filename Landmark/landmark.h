#ifndef LANDMARK_H
#define LANDMARK_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QPointF>

class Landmark : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    explicit Landmark(qreal x, qreal y, qreal w, qreal h, const int ind = 0, QPen pen = QPen(Qt::red), QBrush brush = QBrush(Qt::red));

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setSize(const QSize &size);
    void setIndex(const int &index);

    bool getEnd() const;
    bool getStart() const;
    bool getUp() const;

    void setEnd(const bool value);
    void setStart(const bool value);
    void setUp(const bool value);

    void setSize(const double &size);

signals:
    void changePosition(const QPointF &);

private:
    int index;
    bool up = false;
    bool end = false;
    bool start = false;
    QPen pen;
    QBrush brush;
};

#endif // LANDMARK_H
