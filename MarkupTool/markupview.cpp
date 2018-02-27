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
    QImage imageLoc = image;
    if (contrastImage)
        contrast(imageLoc);
    QPixmap pixmap = QPixmap::fromImage(imageLoc);
    this->image = new QGraphicsPixmapItem(pixmap);
    scene->addItem(this->image);

    if (isScaleSave == true)
        return;

    fitInView (this->image, Qt::KeepAspectRatio); // положение прим смене лица нарушится
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
    if (body->getActivedPart()->pointsSize() < 4){
        int ind = 0;
        for (int indPoint = 0; indPoint < body->getActivedPart()->pointsSize(); indPoint++)
            ind++;
        body->getActivedPart()->addPoint(point, ind);
        if(body->getActivedPart()->pointsSize() == 4)
           updateBodyPath();
        scene->addItem(point);
        return;
    }

    float lineMin = 100000000;
    float indMin = 0;

    for (int indPoint = 0; indPoint < body->getActivedPart()->points.size(); indPoint++){
        int prev = indPoint;
        int next = indPoint+1;
        if (next == body->getActivedPart()->points.size())
            next = 0;

        QPointF prevPoint = body->getActivedPart()->points[prev]->scenePos();
        QPointF nextPoint = body->getActivedPart()->points[next]->scenePos();
        QPointF centerLine = (nextPoint+prevPoint)/2.0;
        float line = sqrt((point->x()-centerLine.x())*(point->x()-centerLine.x()) + (point->y()-centerLine.y())*(point->y()-centerLine.y()));
        //(y1-y2)*x + (x2-x1)*y + (x1*y2-x2*y1);//distFromPoint2SegmentSq(point->scenePos(),body->getActivedPart()->points[prev]->scenePos(), body->getActivedPart()->points[next]->scenePos() );

        if (line < lineMin){
            lineMin = line;
            indMin = prev+1;
        }
    }
    body->getActivedPart()->addPoint(point, indMin);
    updateBodyPath();
    scene->addItem(point);
}



void MarkupView::changeBodyPart(int indPart)
{
    clearScene(body->indActived);
    body->indActived = indPart;
    scaleOnSegment();
    updateBodyPoints();
    updateBodyPath();
}



void MarkupView::setBody(Body newBody)
{
    int indActived = body->indActived;
    clearScene(body->indActived);
    body = new Body();
    body->indActived = indActived;

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
    scaleOnSegment();
}



void MarkupView::setContrast(int value)
{
    this->contrastImage = value;
}



void MarkupView::setScaleParam(const int &param)
{
    scaleSegmentParam = param/10.0;
    scaleOnSegment();
}



void MarkupView::scaleOnSegment()
{
    if (body->getActivedPart()->pointsSize() == 0)
        return;

    QRectF box = body->getActivedPart()->getBox();
    qreal x = box.x() - box.width()*scaleSegmentParam/2.0;
    qreal y = box.y() - box.height()*scaleSegmentParam/2.0;
    qreal w = box.width()*(scaleSegmentParam+1);
    qreal h = box.height()*(scaleSegmentParam+1);
    fitInView (QRectF(x,y,w,h), Qt::KeepAspectRatio);
}



void MarkupView::contrast(QImage &image)
{
    int filter_a [9]= { -1,-1,-1,
                        -1,9,-1,
                       -1,-1,-1};
         int sum = 1;
         int r,g,b;
         QColor color;
         QImage image_new(image.width(),image.height(),QImage::Format_RGB32);

         for(int x = 1; x < image.height()-1; x++){
               for(int y = 1; y<image.width()-1; y++){

                   r = 0;
                   g = 0;
                   b = 0;
                    /*
                   for(int i = 0; i <= 2; i++){
                       for(int j = 0; j <= 2; j++){
                           color = QColor(image.pixel(y+i-1,x+j-1));
                           r += color.red()*(filter_a[(j*3+i)]);
                           g += color.green()*(filter_a[(j*3+i)]);
                           b += color.blue()*(filter_a[(j*3+i)]);
                       }
                   }*/
                    color = QColor(image.pixel(y-1,x-1));
                   r = color.red();//qBound(0, r/sum, 255);
                   g = color.green();//qBound(0, g/sum, 255);
                   b = color.blue();//qBound(0, b/sum, 255);

                   int contrast = (int)((100.000 / 100) * contrastImage);

                   int R3 = contrast < 0 ? (r * (100 - contrast) + 128 * contrast) / 100 : (r * 100 - 128 * contrast) / (101 - contrast);
                   int R4 = R3 < 0 ? R4 = 0 : R3 > 255 ? R4 = 255 : R3;
                   int G3 = contrast < 0 ? (g * (100 - contrast) + 128 * contrast) / 100 : (g * 100 - 128 * contrast) / (101 - contrast);
                   int G4 = G3 < 0 ? G4 = 0 : G3 > 255 ? G4 = 255 : G3;
                   int B3 = contrast < 0 ? (b * (100 - contrast) + 128 * contrast) / 100 : (b * 100 - 128 * contrast) / (101 - contrast);
                   int B4 = B3 < 0 ? B4 = 0 : B3 > 255 ? B4 = 255 : B3;

                   image_new.setPixel(y,x, qRgb(R4,G4,B4));
         }
     }
 image = image_new;
}



void MarkupView::setScaleSave(const bool &isSave)
{
    this->isScaleSave = isSave;
}



QGraphicsScene *MarkupView::getScene()
{
    return scene;
}



double MarkupView::point2SegmentProjectionParameter(const QPointF &point, const QPointF &p1, const QPointF &p2) const
{
    double l2 = sqrt((p1.x()-p2.x())*(p1.x()-p2.x()) + (p1.y()-p2.y())*(p1.y()-p2.y()));
    double product = QVector2D::dotProduct(QVector2D(point-p1), QVector2D(p2 - p1)) / l2;
    return qMax(0.0, qMin(1.0, product));
}



QPointF MarkupView::projectPoint2Segment(const QPointF &point, const QPointF &p1, const QPointF &p2) const
{
    double t = point2SegmentProjectionParameter(point, p1, p2);
    return p1 + t * (p2 - p1);
}



double MarkupView::distFromPoint2SegmentSq(const QPointF &point, const QPointF &p1, const QPointF &p2) const
{
    QPointF projection = projectPoint2Segment(point, p1, p2);
    return sqrt((projection.x()-point.x())*(projection.x()-point.x()) + (projection.y()-point.y())*(projection.y()-point.y())); //(point, projection);
}



void MarkupView::setAllowEdit(bool allow)
{
    this->allowEdit = allow;
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
    body->parts[indPart].update();
}



void MarkupView::clearAllPart(int indPart)
{
    clearScene(indPart);
    clear(indPart);
    Body *b = body;
}



void MarkupView::clearAll()
{
    for (int indPart = 0; indPart < body->parts.size(); indPart++)
        clearAllPart(indPart);
    int indActivedPart = body->indActived;
    body = new Body();
    body->indActived = indActivedPart;
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
    QPen penUp(Qt::green);
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

    if (!isLeftButton || isModifiers || isClickedMark || deleteOption == true || allowEdit == false)
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
