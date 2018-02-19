#include "markupview.h"

MarkupView::MarkupView(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene();
    this->setScene(scene);
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
}



void MarkupView::drawImage(const QImage &image)
{
    clearAll();
    scene->clear();
    QPixmap pixmap = QPixmap::fromImage(image);
    this->image = new QGraphicsPixmapItem(pixmap);
    scene->addItem(this->image);
}



bool MarkupView::clickOnLandmark(const QPointF &point, const float &radius) const
{
    for (int indPoint = 0; indPoint < body->getActivedPart()->points.size(); indPoint++){
        QPointF landmark = body->getActivedPart()->points[indPoint]->scenePos();
        bool amongX = (point.x() < landmark.x()+radius) && (point.x() > landmark.x()-radius);
        bool amongY = (point.y() < landmark.y()+radius) && (point.y() > landmark.y()-radius);
        if (amongX && amongY)
            return true;
    }
    return false;
}



void MarkupView::addLandmark(Landmark *point)
{
    if (body->getActivedPart()->pointsSize() < 3){
        int ind = 0;
        for (int indPoint = 0; indPoint < body->getActivedPart()->pointsSize(); indPoint++)
            ind++;
        body->getActivedPart()->addPoint(point, ind);
    }
    else {
        float lineMin = 100000000;
        float indMin = 0;

        for (int indPoint = 0; indPoint < body->getActivedPart()->points.size(); indPoint++){
            int prev = indPoint;
            int next = indPoint+1;
            if (next == body->getActivedPart()->points.size())
                next = 0;
            float x1 = body->getActivedPart()->points[prev]->x();
            float x2 = body->getActivedPart()->points[next]->x();
            float y1 = body->getActivedPart()->points[prev]->y();
            float y2 = body->getActivedPart()->points[next]->y();
            float x = point->x();
            float y = point->y();

            float line = (y1-y2)*x + (x2-x1)*y + (x1*y2-x2*y1);
            if (line < lineMin){
                lineMin = line;
                indMin = prev+1;
            }

            bool amongX = ((x1 < x) && (x < x2)) || ((x1 > x) && (x > x2));
            bool amongY = ((y1 > y) && (y >y2)) || ((y1 < y) && (y < y2));
            if (amongX && amongY){
                body->getActivedPart()->addPoint(point, indPoint+1);
                scene->addItem(point);
                updateBodyPath();
                return;
            }
        }
        body->getActivedPart()->addPoint(point, indMin);
        updateBodyPath();
    }
    scene->addItem(point);
}



void MarkupView::changeBodyPart(int indPart)
{
    clearScene(body->indActived);
    body->indActived = indPart;
    updateBodyPoints();
    updateBodyPath();
}



void MarkupView::setBody(Body newBody)
{
    clearScene(body->indActived);
    body = new Body();

    for (int indPart = 0; indPart < newBody.parts.size(); indPart++){
        int indPoint = 0;
        if (newBody.parts[indPart].corner.size() == 0)
            continue;
        Landmark *landmark = createLandmark(newBody.parts[indPart].corner[indPoint].x(), newBody.parts[indPart].corner[indPoint].y(), sizeLandmark, sizeLandmark, indPoint);
        landmark->setStart(true);
        body->parts[indPart].loadPoint(landmark, indPoint);

        for (int indPointUp = 0; indPointUp < newBody.parts[indPart].up.size(); indPointUp++){
            indPoint++;
            Landmark *landmark = createLandmark(newBody.parts[indPart].up[indPointUp].x(), newBody.parts[indPart].up[indPointUp].y(), sizeLandmark, sizeLandmark, indPoint);
            body->parts[indPart].loadPoint(landmark, indPoint);
        }

        indPoint++;
        Landmark *landmark2 = createLandmark(newBody.parts[indPart].corner[1].x(), newBody.parts[indPart].corner[1].y(), sizeLandmark, sizeLandmark, indPoint);
        landmark2->setEnd(true);
        body->parts[indPart].loadPoint(landmark2, indPoint);

        for (int indPointDown = 0; indPointDown < newBody.parts[indPart].down.size(); indPointDown++){
            indPoint++;
            Landmark *landmark = createLandmark(newBody.parts[indPart].down[indPointDown].x(), newBody.parts[indPart].down[indPointDown].y(), sizeLandmark, sizeLandmark, indPoint);
            body->parts[indPart].loadPoint(landmark, indPoint);
        }
    }

    updateBodyPoints();
    updateBodyPath();
}



void MarkupView::setDeleteOption(bool option)
{
    this->deleteOption = option;
}



void MarkupView::deleteLandmark(const QPointF &click)
{
    clearScene(body->indActived);
    for (int indPoint = 0; indPoint < body->getActivedPart()->points.size(); indPoint++){
        QPointF landmark = body->getActivedPart()->points[indPoint]->scenePos();
        bool amongX = (click.x() < landmark.x()+sizeLandmark*5) && (click.x() > landmark.x()-sizeLandmark*5);
        bool amongY = (click.y() < landmark.y()+sizeLandmark*5) && (click.y() > landmark.y()-sizeLandmark*5);
        if (amongX && amongY){
            body->getActivedPart()->deletePoint(indPoint);
        }
    }
    updateBodyPoints();
    updateBodyPath();
}



void MarkupView::clearScenePoints(int indPart)
{
    for (int indItem = 0; indItem < body->parts[indPart].pointsSize(); indItem++)
        scene->removeItem(body->parts[indPart].points[indItem]);
}



void MarkupView::clearScenePath(int indPart)
{
    scene->removeItem(body->parts[indPart].pathUp);
    scene->removeItem(body->parts[indPart].pathDown);
}



void MarkupView::clearScene(int indPart)
{
    clearScenePoints(indPart);
    clearScenePath(indPart);
}



void MarkupView::clearPoints(int indPart)
{
    body->parts[indPart].points.clear();
}



void MarkupView::clearPath(int indPart)
{
    body->parts[indPart].pathUp = nullptr;
    body->parts[indPart].pathDown = nullptr;
}



void MarkupView::clear(int indPart)
{
    clearPoints(indPart);
    clearPath(indPart);
}



void MarkupView::clearAllPart(int indPart)
{
    clearScene(indPart);
    clear(indPart);
}



void MarkupView::clearAll()
{
    for (int indPart = 0; indPart < body->parts.size(); indPart++)
        clearAllPart(indPart);
    body = new Body();
}



Body *MarkupView::getBody()
{
    return body;
}



Landmark *MarkupView::createLandmark(qreal x, qreal y, qreal w, qreal h, int ind, QPen pen, QBrush brush)
{
    Landmark *landmark = new Landmark(x, y, w, h, ind, pen, brush);
    connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updateBodyPath()));
    return landmark;
}



void MarkupView::changeSizeLandmark(const double &size)
{
    for (int indPoint = 0; indPoint < body->getActivedPart()->pointsSize(); indPoint++)
        body->getActivedPart()->points[indPoint]->setSize(size);
    sizeLandmark = size;
}



void MarkupView::updateBodyPath()
{
    body->getActivedPart()->update();
    clearScenePath(body->indActived);

    if (body->getActivedPart()->pointsSize() < 4)
        return;

    QVector<QPointF> pointsUp = body->getActivedPart()->up;
    QVector<QPointF> pointsCorners = body->getActivedPart()->corner;
    QVector<QPointF> pointsDown = body->getActivedPart()->down;

    pointsUp.push_front(pointsCorners.first());
    pointsUp.push_back(pointsCorners.last());
    QPainterPath pathUp = Spline::build(QPolygonF(pointsUp));

    body->getActivedPart()->pathUp = new QGraphicsPathItem(pathUp);
    QPen penUp(Qt::red);
    penUp.setWidth(0.1);
    body->getActivedPart()->pathUp->setPen(penUp);
    scene->addItem(body->getActivedPart()->pathUp);

    pointsDown.push_front(pointsCorners.last());
    pointsDown.push_back(pointsCorners.first());
    QPainterPath pathDown = Spline::build(QPolygonF(pointsDown));

    body->getActivedPart()->pathDown = new QGraphicsPathItem(pathDown);
    QPen penDown(Qt::blue);
    penDown.setWidth(0.1);
    body->getActivedPart()->pathDown->setPen(penDown);
    scene->addItem(body->getActivedPart()->pathDown);

}



void MarkupView::updateBodyPoints()
{
    clearScenePoints(body->indActived);
    for (int indPoint = 0; indPoint < body->getActivedPart()->pointsSize(); indPoint++)
        scene->addItem(body->getActivedPart()->points[indPoint]);
}



void MarkupView::wheelEvent(QWheelEvent *event)
{
    const float scaleFactor = 1.15;

    if (event->delta() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}



void MarkupView::mousePressEvent(QMouseEvent *event)
{
    if (event->modifiers() == Qt::ShiftModifier)
        this->setDragMode(DragMode::ScrollHandDrag);
    else
        this->setDragMode(DragMode::NoDrag);

    if (deleteOption == false)
        QGraphicsView::mousePressEvent(event);

    if (deleteOption == true)
        viewport()->setCursor(Qt::PointingHandCursor);
    else if (event->modifiers() != Qt::ShiftModifier)
        viewport()->setCursor(Qt::CrossCursor);

    bool isLeftButton = (event->button() == Qt::LeftButton);
    bool isModifiers = (event->modifiers() == Qt::ShiftModifier);
    bool isClickedMark = clickOnLandmark(mapToScene(event->pos()), sizeLandmark*5);

    if (!isLeftButton || isModifiers || isClickedMark || deleteOption == true)
        return;

    QPointF position = mapToScene(event->pos());
    Landmark *landmark = createLandmark(position.x(), position.y(), sizeLandmark, sizeLandmark);
    addLandmark(landmark);
}



void MarkupView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    if (deleteOption == true)
        viewport()->setCursor(Qt::PointingHandCursor);
    else if (event->modifiers() != Qt::ShiftModifier)
        viewport()->setCursor(Qt::CrossCursor);
}



void MarkupView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if (deleteOption == true)
        viewport()->setCursor(Qt::PointingHandCursor);
    else if (event->modifiers() != Qt::ShiftModifier)
        viewport()->setCursor(Qt::CrossCursor);
}



void MarkupView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
    if (deleteOption == true)
        viewport()->setCursor(Qt::PointingHandCursor);
    else if (event->modifiers() != Qt::ShiftModifier)
        viewport()->setCursor(Qt::CrossCursor);

    bool isLeftButton = (event->button() == Qt::LeftButton);
    bool isClickedMark = clickOnLandmark(mapToScene(event->pos()), sizeLandmark*5);

    if (!isLeftButton || !isClickedMark || deleteOption == false)
        return;

    deleteLandmark(mapToScene(event->pos()));
}
