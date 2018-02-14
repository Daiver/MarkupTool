#include "markupview.h"



MarkupView::MarkupView(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene();
    this->setScene(scene);
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
}



void MarkupView::drawImage(const QImage &image)
{
    scene->clear();
    clearBodyParts();
    QPixmap pixmap = QPixmap::fromImage(image);
    imageOpened = new QGraphicsPixmapItem(pixmap);
    scene->addItem(imageOpened);
}


void MarkupView::updateBodyPart()
{
    for (int indPoint = 0; indPoint < body->parts[body->indActived].points.size();indPoint++)
        scene->addItem(body->parts[body->indActived].points[indPoint]);
    updatePart();
}



void MarkupView::clearScene()
{
    scene->removeItem(body->parts[body->indActived].pathUp);
    scene->removeItem(body->parts[body->indActived].pathDown);
    for (int indItem = 0; indItem < body->parts[body->indActived].points.size(); indItem++)
        scene->removeItem(body->parts[body->indActived].points[indItem]);
}



void MarkupView::updatePart()
{
    if (body->parts[body->indActived].points.size() < 4)
        return;

    scene->removeItem(body->parts[body->indActived].pathUp);
    scene->removeItem(body->parts[body->indActived].pathDown);

    QPainterPath pathUp(body->parts[body->indActived].points[0]->scenePos());
    int indEnd = 0;
    for (int indPoint = 1; indPoint < body->parts[body->indActived].points.size(); indPoint++){
        pathUp.lineTo(body->parts[body->indActived].points[indPoint]->scenePos());
        if (body->parts[body->indActived].points[indPoint]->end == true){
            indEnd = indPoint;
            break;
        }
    }

    body->parts[body->indActived].pathUp = new QGraphicsPathItem(pathUp);
    QPen pen(Qt::red);
    pen.setWidth(0.1);
    body->parts[body->indActived].pathUp->setPen(pen);
    scene->addItem(body->parts[body->indActived].pathUp);

    QPainterPath pathDown(body->parts[body->indActived].points[indEnd]->scenePos());
    for (int indPoint = indEnd; indPoint < body->parts[body->indActived].points.size(); indPoint++)
        pathDown.lineTo(body->parts[body->indActived].points[indPoint]->scenePos());
    pathDown.lineTo(body->parts[body->indActived].points[0]->scenePos());

    body->parts[body->indActived].pathDown = new QGraphicsPathItem(pathDown);
    QPen pen2(Qt::blue);
    pen2.setWidth(0.1);
    body->parts[body->indActived].pathDown->setPen(pen2);
    scene->addItem(body->parts[body->indActived].pathDown);


    /*
    scene->removeItem(body->parts[body->indActived].path);
    QPainterPath pathUp(body->parts[body->indActived].points[0]->scenePos());
    for (int indLeftEye = 1; indLeftEye < body->parts[body->indActived].points.size(); indLeftEye++)
        pathUp.lineTo(body->parts[body->indActived].points[indLeftEye]->scenePos());
    pathUp.lineTo(body->parts[body->indActived].points[0]->scenePos());
    body->parts[body->indActived].path = new QGraphicsPathItem(pathUp);
    QPen pen(Qt::red);
    pen.setWidth(0.1);
    body->parts[body->indActived].path->setPen(pen);
    scene->addItem(body->parts[body->indActived].path);
    */
}



void MarkupView::updateBody(Body newBody)
{
    clearScene();
    body = new Body();

    // Всё ровно это все еще говно, как и куча кода до этого, вот нахера я торопился?!

    for (int indPart = 0; indPart < newBody.parts.size(); indPart++){
        int indPoint = 0;
        if (newBody.parts[indPart].corner.size() == 0)
            continue;
        Landmark *landmark = new Landmark(newBody.parts[indPart].corner[indPoint].x()-0, newBody.parts[indPart].corner[indPoint].y()-0, 2, 2, indPoint);
        landmark->start = true;
        connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        body->parts[indPart].loadPoint(landmark, indPoint);

        for (int indPointUp = 0; indPointUp < newBody.parts[indPart].up.size(); indPointUp++){
            indPoint++;
            Landmark *landmark = new Landmark(newBody.parts[indPart].up[indPointUp].x()-0, newBody.parts[indPart].up[indPointUp].y()-0, 2, 2, indPoint);
            connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
            body->parts[indPart].loadPoint(landmark, indPoint);
        }

        indPoint++;
        Landmark *landmark2 = new Landmark(newBody.parts[indPart].corner[1].x()-0, newBody.parts[indPart].corner[1].y()-0, 2, 2, indPoint);
        landmark2->end = true;
        connect(landmark2, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        body->parts[indPart].loadPoint(landmark2, indPoint);

        for (int indPointDown = 0; indPointDown < newBody.parts[indPart].down.size(); indPointDown++){
            indPoint++;
            Landmark *landmark = new Landmark(newBody.parts[indPart].down[indPointDown].x()-0, newBody.parts[indPart].down[indPointDown].y()-0, 2, 2, indPoint);
            connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
            body->parts[indPart].loadPoint(landmark, indPoint);
        }
    }

    updateBodyPart();
}



void MarkupView::clearBodyPart()
{
    scene->removeItem(body->parts[body->indActived].pathUp);
    scene->removeItem(body->parts[body->indActived].pathDown);
    for (int indItem = 0; indItem < body->parts[body->indActived].points.size(); indItem++)
        scene->removeItem(body->parts[body->indActived].points[indItem]);

    body->parts[body->indActived].pathUp = nullptr;
    body->parts[body->indActived].pathDown = nullptr;
    body->parts[body->indActived].points.clear();
}



void MarkupView::clearBodyParts()
{
    for (int indPart = 0; indPart < body->parts.size(); indPart++){
        scene->removeItem(body->parts[indPart].pathUp);
        scene->removeItem(body->parts[indPart].pathDown);
        for (int indItem = 0; indItem < body->parts[indPart].points.size(); indItem++)
            scene->removeItem(body->parts[indPart].points[indItem]);

        body->parts[indPart].pathUp = nullptr;
        body->parts[indPart].pathDown = nullptr;
        body->parts[indPart].points.clear();
    }
}



float MarkupView::getDistance(const QPointF &point, const QPointF &p1, const QPointF &p2) const
{
    float a = std::abs((p2.y() - p1.y())*point.x() - (p2.x()-p1.x())*point.y() + p2.x()*p1.y() - p2.y()*p1.x());
    float b = sqrt(pow(p2.y()-p1.y(), 2) + pow(p2.x()-p1.x(), 2));
    return a/b;
}



float MarkupView::getSegmentProjParam(const QPointF &point, const QPointF &p1, const QPointF &p2) const
{
    float evLong = sqrt(pow(p1.x()-p1.y(), 2) + pow(p2.x()-p2.y(), 2));
    float dotProd = QPointF::dotProduct(point-p1, p2 - p1)/evLong;
    float one = 1;
    float zero = 0;
    float min = std::min(one, dotProd);
    return std::max(zero,min);
}



QPointF MarkupView::projectPoint2Segment(const QPointF &point, const QPointF &p1, const QPointF &p2) const
{
    float t = getSegmentProjParam(point, p1, p2);
    return p1 + t * (p2 - p1);
}



float MarkupView::distFromPoint2SegmentSq(const QPointF &point, const QPointF &p1, const QPointF &p2) const
{
    QPointF projection = projectPoint2Segment(point, p1, p2);
    float evLong = sqrt(pow(point.x()-point.y(), 2) + pow(projection.x()-projection.y(), 2));
    return evLong;
}



bool MarkupView::clickOnLandmark(const QPointF &point)
{
    for (int indPoint = 0; indPoint < body->parts[body->indActived].points.size(); indPoint++){
        QPointF landmark = body->parts[body->indActived].points[indPoint]->scenePos();
        bool amongX = (point.x() < landmark.x()+5) && (point.x() > landmark.x()-5);
        bool amongY = (point.y() < landmark.y()+5) && (point.y() > landmark.y()-5);
        if (amongX && amongY)
            return true;
    }
    return false;
}



QVector<QPointF> MarkupView::getBodyPart(int indBlock) const
{
    QVector<QPointF> leftEye;
    for (int indPoint = 0; indPoint < body->parts[indBlock].points.size(); indPoint++)
        leftEye.push_back(body->parts[indBlock].points[indPoint]->scenePos());
    return leftEye;
}



QVector<QPointF> MarkupView::getUpPart(int indBLock) const
{
    QVector<QPointF> upPart;
    for (int indPoint = 1; indPoint < body->parts[indBLock].points.size(); indPoint++){
        if (body->parts[indBLock].points[indPoint]->end == true)
            break;
        upPart.push_back(body->parts[indBLock].points[indPoint]->scenePos());
    }
    return upPart;
}



QVector<QPointF> MarkupView::getDownPart(int indBLock) const
{
    QVector<QPointF> downPart;

    int indEnd;
    for (int indPoint = 0; indPoint < body->parts[indBLock].points.size(); indPoint++)
        if (body->parts[indBLock].points[indPoint]->end == true)
            indEnd = indPoint;

    for (int indPoint = indEnd+1; indPoint < body->parts[indBLock].points.size(); indPoint++){
        if (body->parts[indBLock].points[indPoint]->start == true)
            break;
        downPart.push_back(body->parts[indBLock].points[indPoint]->scenePos());
    }
    return downPart;
}



QVector<QPointF> MarkupView::getCorner(int indBLock) const
{
    QVector<QPointF> corners;

    for (int indPoint = 0; indPoint < body->parts[indBLock].points.size(); indPoint++)
        if (body->parts[indBLock].points[indPoint]->start == true)
            corners.push_back(body->parts[indBLock].points[indPoint]->scenePos());

    for (int indPoint = 0; indPoint < body->parts[indBLock].points.size(); indPoint++)
        if (body->parts[indBLock].points[indPoint]->end == true)
            corners.push_back(body->parts[indBLock].points[indPoint]->scenePos());

    return corners;
}


void MarkupView::addPointInPart(Landmark *landmark)
{
    /*
    float indMin = 0;
    float lineMin = 10000000000;

    if (body->parts[body->indActived].points.size() < 4){
      body->parts[body->indActived].addPoint(landmark, body->parts[body->indActived].points.size());
      return;
    }


    for (int indPoint = 0; indPoint < body->parts[body->indActived].points.size(); indPoint++){
        int prev = indPoint;
        int next = indPoint+1;
        if (next == body->parts[body->indActived].points.size())
            next = 0;
        QPointF pt = landmark->scenePos();
        QPointF pt1 = body->parts[body->indActived].points[prev]->scenePos();
        QPointF pt2 = body->parts[body->indActived].points[next]->scenePos();


        float line = distFromPoint2SegmentSq(pt, pt1, pt2);
        if (line < lineMin){
            lineMin = line;
            indMin = prev+1;
        }
    }

    body->parts[body->indActived].addPoint(landmark, indMin);
    */

    int indInsert = 0;
    float lineMin = 100000000;
    float indMin = 0;

    for (int indPoint = 0; indPoint < body->parts[body->indActived].points.size(); indPoint++){
        int prev = indPoint;
        int next = indPoint+1;
        if (next == body->parts[body->indActived].points.size())
            next = 0;
        float x1 = body->parts[body->indActived].points[prev]->x();
        float x2 = body->parts[body->indActived].points[next]->x();
        float y1 = body->parts[body->indActived].points[prev]->y();
        float y2 = body->parts[body->indActived].points[next]->y();
        float x = landmark->x();
        float y = landmark->y();
        indInsert = prev+1;

        float line = (y1-y2)*x + (x2-x1)*y + (x1*y2-x2*y1);
        if (line < lineMin){
            lineMin = line;
            indMin = prev+1;
        }

        bool amongX = ((x1 < x) && (x < x2)) || ((x1 > x) && (x > x2));
        bool amongY = ((y1 > y) && (y >y2)) || ((y1 < y) && (y < y2));
        if (amongX && amongY){
            body->parts[body->indActived].addPoint(landmark, indPoint+1);
            return;
        }
    }
    body->parts[body->indActived].addPoint(landmark, indMin);

}



void MarkupView::changeBodyPart(int indBlock)
{
    clearScene();
    body->indActived = indBlock;
    updateBodyPart();
}



void MarkupView::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;
    if (event->delta() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}



void MarkupView::mousePressEvent(QMouseEvent *event)
{
    if (event->modifiers() == Qt::ShiftModifier)
        this->setDragMode(DragMode::ScrollHandDrag);

    QGraphicsView::mousePressEvent(event);
    if (event->modifiers() != Qt::ShiftModifier)
        viewport()->setCursor(Qt::CrossCursor);
    if (event->button() != Qt::LeftButton || event->modifiers() == Qt::ShiftModifier || clickOnLandmark(mapToScene(event->pos())))
        return;

    QPointF position = mapToScene(event->pos());
    Landmark *landmark = new Landmark(position.x(), position.y(), 2, 2, 0);
    connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
    addPointInPart(landmark);
    scene->addItem(landmark);

    updatePart();

}



void MarkupView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    if (event->modifiers() != Qt::ShiftModifier)
        viewport()->setCursor(Qt::CrossCursor);
    //qDebug() << mapToScene(event->pos());
}
