#include "markupview.h"
#include "math.h"

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
    bright(imageLoc);
    contrast(imageLoc);
    QPixmap pixmap = QPixmap::fromImage(imageLoc);
    this->imageItem = new QGraphicsPixmapItem(pixmap);
    scene->addItem(this->imageItem);

    if (scaleSaveOption == true)
        return;

    fitInView (this->imageItem, Qt::KeepAspectRatio); // положение прим смене лица нарушится
}



bool MarkupView::clickOnLandmark(const QPointF &point, const float &radius) const
{
    for (int indPoint = 0; indPoint < faceShape->getActivedPart()->points.size(); indPoint++){
        QPointF landmark = faceShape->getActivedPart()->points[indPoint]->scenePos();
        bool amongX = (point.x() < landmark.x()+radius) && (point.x() > landmark.x()-radius);
        bool amongY = (point.y() < landmark.y()+radius) && (point.y() > landmark.y()-radius);
        if (amongX && amongY)
            return true;
    }
    return false;
}



void MarkupView::addLandmark(Landmark *point, bool updatePath)
{

    if (faceShape->getActivedPart()->pointsSize() < 4){
        int ind = 0;
        for (int indPoint = 0; indPoint < faceShape->getActivedPart()->pointsSize(); indPoint++)
            ind++;
        faceShape->getActivedPart()->addPoint(point, ind);
        if(faceShape->getActivedPart()->pointsSize() == 4 && updatePath)
            updateSegmentPath();
        scene->addItem(point);
        return;
    }

    float lineMin = 100000000;
    float indMin = 0;

    for (int indPoint = 0; indPoint < faceShape->getActivedPart()->points.size(); indPoint++){
        int prev = indPoint;
        int next = indPoint+1;
        if (next == faceShape->getActivedPart()->points.size())
            next = 0;

        QPointF prevPoint = faceShape->getActivedPart()->points[prev]->scenePos();
        QPointF nextPoint = faceShape->getActivedPart()->points[next]->scenePos();

        float line = getDistance(point->scenePos(), prevPoint, nextPoint);
        if (line < lineMin){
            lineMin = line;
            indMin = prev+1;
        }
    }
    faceShape->getActivedPart()->addPoint(point, indMin);
    if (updatePath)
        updateSegmentPath();
    scene->addItem(point);
}



void MarkupView::changeSegment(const int &indSegment)
{
    clearScene(faceShape->indActived);
    faceShape->indActived = indSegment;

    scaleOnSegment();
    updateSegmentPoints();
    updateSegmentPath();
}



void MarkupView::setFaceShape(ShapeFace newFaceShape)
{
    int indActived = faceShape->indActived;
    clearScene(faceShape->indActived);
    faceShape = new ShapeFace();
    faceShape->indActived = indActived;

    for (int indPart = 0; indPart < newFaceShape.parts.size(); indPart++){
        Segment part = newFaceShape.parts[indPart];
        faceShape->indActived = indPart;
        faceShape->getActivedPart()->indInvisibleSegment = 0;
        if (part.corner.size() == 0)
            continue;

        Landmark *cornerLeft = createLandmark(part.corner[0].x(), part.corner[0].y(), sizePointsValue, sizePointsValue, indPart);
        Landmark *centralUp = createLandmark(part.centers[0].x(), part.centers[0].y(), sizePointsValue, sizePointsValue, indPart);
        Landmark *cornerRight = createLandmark(part.corner[1].x(), part.corner[1].y(), sizePointsValue, sizePointsValue, indPart);
        Landmark *centralDown = createLandmark(part.centers[1].x(), part.centers[1].y(), sizePointsValue, sizePointsValue, indPart);

        faceShape->parts[indPart].invisibleSegments = part.invisibleSegments;
        addLandmark(cornerLeft, false);
        addLandmark(centralUp, false);
        addLandmark(cornerRight, false);
        addLandmark(centralDown, false);

        for (int indPoint = 0; indPoint < part.up.size(); indPoint++){
            if (faceShape->parts[indPart].up.contains(part.up[indPoint]))
                continue;
            Landmark *landmark = createLandmark(part.up[indPoint].x(), part.up[indPoint].y(), sizePointsValue, sizePointsValue);
            addLandmark(landmark, false);
        }

        for (int indPoint = 0; indPoint < part.down.size(); indPoint++){
            if (faceShape->parts[indPart].down.contains(part.down[indPoint]))
                continue;
            Landmark *landmark = createLandmark(part.down[indPoint].x(), part.down[indPoint].y(), sizePointsValue, sizePointsValue);
            addLandmark(landmark, false);
        }
    }

    for (int indPart = 0; indPart < faceShape->parts.size(); indPart++)
        for (int indInvSeg = 0; indInvSeg < newFaceShape.parts[indPart].invisibleSegments.size(); indInvSeg++)
            for (int ind = 0; ind < newFaceShape.parts[indPart].invisibleSegments[indInvSeg].size(); ind++)
                faceShape->parts[indPart].points[newFaceShape.parts[indPart].invisibleSegments[indInvSeg].at(ind)]->setInvsibile(true);


    faceShape->indActived = indActived;
    updateSegmentPoints();
    updateSegmentPath();
    scaleOnSegment();
}



void MarkupView::updateImage(const QImage &image)
{
    scene->removeItem(this->imageItem);
    QImage imageLoc = image;
    bright(imageLoc);
    contrast(imageLoc);
    QPixmap pixmap = QPixmap::fromImage(imageLoc);
    this->imageItem = new QGraphicsPixmapItem(pixmap);
    this->imageItem->setZValue(-100);
    scene->addItem(this->imageItem);

    if (scaleSaveOption == true)
        return;

    updateSegmentPoints();
    updateSegmentPath();
    fitInView (this->imageItem, Qt::KeepAspectRatio); // положение прим смене лица нарушится
}



void MarkupView::setContrast(const int &value)
{
    this->contrastValue = value;
}



int MarkupView::getBright() const
{
    return brightValue;
}



void MarkupView::setInvisibleSegmentPoint(const QPointF &point)
{
    clearScene(faceShape->indActived);
    for (int indPoint = 0; indPoint < faceShape->getActivedPart()->points.size(); indPoint++){
        QPointF landmark = faceShape->getActivedPart()->points[indPoint]->scenePos();
        bool amongX = (point.x() < landmark.x()+sizePointsValue*5) && (point.x() > landmark.x()-sizePointsValue*5);
        bool amongY = (point.y() < landmark.y()+sizePointsValue*5) && (point.y() > landmark.y()-sizePointsValue*5);
        if (amongX && amongY){
            if (faceShape->getActivedPart()->points[indPoint]->isInvisble() == true)
                faceShape->getActivedPart()->setInvisibleSegment(indPoint, false);
            else
                faceShape->getActivedPart()->setInvisibleSegment(indPoint, invisibleSegmentOption);
        }
    }
    updateSegmentPoints();
    updateSegmentPath();
}



void MarkupView::setInvisibleSegmentOption(const bool &value)
{
    invisibleSegmentOption = value;
}



void MarkupView::setBright(const int &value)
{
    this->brightValue = value;
}



void MarkupView::setScaleParam(const int &value)
{
    scaleSegmentValue = value/10.0;
    scaleOnSegment();
}



void MarkupView::scaleOnSegment()
{
    if (faceShape->getActivedPart()->pointsSize() == 0)
        return;

    QRectF box = faceShape->getActivedPart()->getBox();
    qreal x = box.x() - box.width()*scaleSegmentValue/2.0;
    qreal y = box.y() - box.height()*scaleSegmentValue/2.0;
    qreal w = box.width()*(scaleSegmentValue+1);
    qreal h = box.height()*(scaleSegmentValue+1);
    fitInView (QRectF(x,y,w,h), Qt::KeepAspectRatio);
}



int MarkupView::getContrast() const
{
    return contrastValue;
}



void MarkupView::contrast(QImage &image)
{
    int r,g,b;
    QColor color;
    QImage image_new(image.width(),image.height(),QImage::Format_RGB32);

    for(int x = 0; x < image.height(); x++){
        for(int y = 0; y<image.width(); y++){
            color = QColor(image.pixel(y,x));
            r = color.red();//qBound(0, r/sum, 255);
            g = color.green();//qBound(0, g/sum, 255);
            b = color.blue();//qBound(0, b/sum, 255);

            int contrast = (int)((100.000 / 100) * contrastValue);

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



void MarkupView::bright(QImage &image)
{
    int r,g,b;
    QColor color;
    QImage image_new(image.width(),image.height(),QImage::Format_RGB32);

    for(int x = 0; x < image.height(); x++){
        for(int y = 0; y<image.width(); y++){
            color = QColor(image.pixel(y,x));
            r = color.red();//qBound(0, r/sum, 255);
            g = color.green();//qBound(0, g/sum, 255);
            b = color.blue();//qBound(0, b/sum, 255);

            int bright = (int)((100.000 / 100) * brightValue);


            int R2 = r + bright * 128 / 100 <= 0 ? 0 : r + bright * 128 / 100 >= 255 ? 255 : r + bright * 128 / 100;
            int G2 = g + bright * 128 / 100 <= 0 ? 0 : g + bright * 128 / 100 >= 255 ? 255 : g + bright * 128 / 100;
            int B2 = b + bright * 128 / 100 <= 0 ? 0 : b + bright * 128 / 100 >= 255 ? 255 : b + bright * 128 / 100;

            image_new.setPixel(y,x, qRgb(R2,G2,B2));
        }
    }
    image = image_new;
}



float MarkupView::getDistance(const QPointF &point, const QPointF &p1, const QPointF &p2) const
{
    QVector2D lineV(p2 - p1);
    QVector2D pt(point - p1);
    QVector2D project = QVector2D::dotProduct(pt,lineV)*lineV/QVector2D::dotProduct(lineV,lineV);//lineV*(QVector2D::dotProduct(lineV, pt)/QVector2D::dotProduct(lineV, lineV));
    QVector2D d = project+QVector2D(p1);
    if (d.x() < p1.x() && p1.x() < p2.x())
        d = QVector2D(p1);
    if (d.x() > p2.x() && p1.x() < p2.x())
        d = QVector2D(p2);
    if (d.x() > p1.x() && p1.x() > p2.x())
        d = QVector2D(p1);
    if (d.x() < p2.x() && p1.x() > p2.x())
        d = QVector2D(p2);
    float line = sqrt((point.x()-d.x())*(point.x()-d.x()) + (point.y()-d.y())*(point.y()-d.y()));//sqrt(QVector2D::dotProduct(QVector2D(point.scenePos()),d));
    return line;
}



void MarkupView::setScaleSave(const bool &value)
{
    this->scaleSaveOption = value;
}



QGraphicsScene *MarkupView::getScene()
{
    return scene;
}



void MarkupView::setAllowEdit(const bool &value)
{
    this->allowEditOption = value;
}



void MarkupView::setDeleteOption(const bool &value)
{
    this->deleteOption = value;
}



void MarkupView::deleteLandmark(const QPointF &click)
{
    clearScene(faceShape->indActived);
    for (int indPoint = 0; indPoint < faceShape->getActivedPart()->points.size(); indPoint++){
        QPointF landmark = faceShape->getActivedPart()->points[indPoint]->scenePos();
        bool amongX = (click.x() < landmark.x()+sizePointsValue*5) && (click.x() > landmark.x()-sizePointsValue*5);
        bool amongY = (click.y() < landmark.y()+sizePointsValue*5) && (click.y() > landmark.y()-sizePointsValue*5);
        if (amongX && amongY){
            faceShape->getActivedPart()->deletePoint(indPoint);
        }
    }
    updateSegmentPoints();
    updateSegmentPath();
}



void MarkupView::clearScenePoints(const int &indPart)
{
    for (int indItem = 0; indItem < faceShape->parts[indPart].pointsSize(); indItem++)
        scene->removeItem(faceShape->parts[indPart].points[indItem]);
}



void MarkupView::clearScenePath(const int &indPart)
{
    scene->removeItem(faceShape->parts[indPart].pathUp);
    scene->removeItem(faceShape->parts[indPart].pathDown);
    for (int indPath = 0; indPath < paths.size(); indPath++){
        scene->removeItem(paths[indPath]);
    }
}



void MarkupView::clearScene(const int &indPart)
{
    clearScenePoints(indPart);
    clearScenePath(indPart);
}



void MarkupView::clearPoints(const int &indPart)
{
    faceShape->parts[indPart].points.clear();
    faceShape->parts[indPart].invisibleSegments.clear();
    faceShape->parts[indPart].indInvisibleSegment = 0;
}



void MarkupView::clearPath(const int &indPart)
{
    faceShape->parts[indPart].pathUp = nullptr;
    faceShape->parts[indPart].pathDown = nullptr;
}



void MarkupView::clear(const int &indPart)
{
    clearPoints(indPart);
    clearPath(indPart);
    faceShape->parts[indPart].update();
}



void MarkupView::clearAllPart(const int &indPart)
{
    clearScene(indPart);
    clear(indPart);
}



void MarkupView::clearAll()
{
    for (int indPart = 0; indPart < faceShape->parts.size(); indPart++)
        clearAllPart(indPart);
    int indActivedPart = faceShape->indActived;
    faceShape = new ShapeFace();
    faceShape->indActived = indActivedPart;
}



ShapeFace *MarkupView::getFaceShape()
{
    return faceShape;
}



Landmark *MarkupView::createLandmark(qreal x, qreal y, qreal w, qreal h, int ind, QPen pen, QBrush brush)
{
    Landmark *landmark = new Landmark(x, y, w, h, ind, pen, brush);
    connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updateSegmentPath()));
    return landmark;
}



void MarkupView::changeSizeLandmark(const double &size)
{
    for (int indPoint = 0; indPoint < faceShape->getActivedPart()->pointsSize(); indPoint++)
        faceShape->getActivedPart()->points[indPoint]->setSize(size);
    sizePointsValue = size;
}



void MarkupView::updateSegmentPath()
{
    faceShape->getActivedPart()->update();
    clearScenePath(faceShape->indActived);

    if (faceShape->getActivedPart()->pointsSize() < 4)
        return;

    QVector<QPointF> pointsUp = faceShape->getActivedPart()->up;
    QVector<QPointF> pointsCorners = faceShape->getActivedPart()->corner;
    QVector<QPointF> pointsDown = faceShape->getActivedPart()->down;

    QList<QVector<int>> invisibleSegmentInd = faceShape->getActivedPart()->invisibleSegments;

    pointsUp.push_front(pointsCorners.first());
    pointsUp.push_back(pointsCorners.last());

    //    const int size = pointsUp.size();
    //    Spline<QVector2D> splineUp;
    //    for (int indPoint = 0; indPoint < pointsUp.size(); indPoint++){
    //        splineUp.add(QVector2D(pointsUp[indPoint]));
    //    }

    //    QPainterPath pathUp;
    //    pathUp.moveTo(pointsUp[0].x(), pointsUp[0].y());
    //    float len = pointsUp.last().x() - pointsUp.first().x();
    //    len = qFabs(len);

    //    for (float x = 0; x < splineUp.length(); x = x + 1.0/(len/5)){
    //        QVector2D pt = splineUp.at(x);
    //        pathUp.lineTo(pt.toPointF());
    //    }

    //QPainterPath pathUp ;//= Spline::build(QPolygonF(pointsUp));

    //    //
    //    tinyspline::BSpline bspline(pointsUp.size());
    //    std::vector<tinyspline::real> ctrlp = bspline.ctrlp();
    //    int indCtrl = 0;
    //    for (int ind = 0; ind < ctrlp.size()-1; ind = ind+2){
    //        ctrlp[ind] = pointsUp[indCtrl].x();
    //        ctrlp[ind+1] = pointsUp[indCtrl].y();
    //        indCtrl++;
    //    }
    //    bspline.setCtrlp(ctrlp);
    //    //tinyspline::BSpline beziers = bspline.derive().toBeziers();
    //    QPainterPath pathUp;
    //    pathUp.moveTo(pointsUp[0].x(), pointsUp[0].y());

    //    for (double x = pointsUp[0].x()+1; x <= pointsUp.last().x(); x = x + 1){
    //        std::vector<tinyspline::real> result;
    //        result =bspline.evaluate(x).result(); // beziers(x).result();
    //        pathUp.lineTo(x, result[result.size()-1]);
    //    }
    //
    SplineAdapter splineUp;
    splineUp.setPoints(pointsUp);
    QPainterPath pathUp = splineUp.getPath();

    faceShape->getActivedPart()->pathUp = new QGraphicsPathItem(pathUp);
    QPen penUp(Qt::green);
    penUp.setWidth(0.1);
    faceShape->getActivedPart()->pathUp->setPen(penUp);
    scene->addItem(faceShape->getActivedPart()->pathUp);

    pointsDown.push_front(pointsCorners.last());
    pointsDown.push_back(pointsCorners.first());
    //QPainterPath pathDown; //= Spline::build(QPolygonF(pointsDown));
    SplineAdapter splineDown;
    splineDown.setPoints(pointsDown);
    QPainterPath pathDown = splineDown.getPath();

    faceShape->getActivedPart()->pathDown = new QGraphicsPathItem(pathDown);
    QPen penDown(Qt::blue);
    penDown.setWidth(0.1);
    faceShape->getActivedPart()->pathDown->setPen(penDown);
    scene->addItem(faceShape->getActivedPart()->pathDown);

    if (invisibleSegmentInd.size() != 0){
        int size = invisibleSegmentInd.size();

        for (int indSegment = 0; indSegment < size; indSegment++){
            for (int indPoint = 0; indPoint < invisibleSegmentInd[indSegment].size()-1; indPoint++){
                int indFirstPoint = invisibleSegmentInd[indSegment].at(indPoint);
                int indSecondPoint = invisibleSegmentInd[indSegment].at(indPoint+1);
                QPointF firstPoint = faceShape->getActivedPart()->points[indFirstPoint]->scenePos();
                QPointF secondPoint = faceShape->getActivedPart()->points[indSecondPoint]->scenePos();

                QPainterPath path;
                if (!faceShape->getActivedPart()->down.contains(firstPoint))
                    path = splineUp.getPath(firstPoint.x(), secondPoint.x());
                else
                    path = splineDown.getPath(firstPoint.x(), secondPoint.x());

                paths.push_back(new QGraphicsPathItem(path));
                QPen penUp;
                penUp.setColor(Qt::white);
                if (faceShape->getActivedPart()->indInvisibleSegment == indSegment)
                    penUp.setColor(Qt::yellow);
                penUp.setWidth(1);
                paths.last()->setPen(penUp);
                scene->addItem(paths.last());
            }
        }
    }

}



void MarkupView::updateSegmentPoints()
{
    clearScenePoints(faceShape->indActived);

    for (int indPoint = 0; indPoint < faceShape->getActivedPart()->pointsSize(); indPoint++)
        scene->addItem(faceShape->getActivedPart()->points[indPoint]);
}



void MarkupView::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
        scale(scaleValue, scaleValue);
    else
        scale(1.0 / scaleValue, 1.0 / scaleValue);
}



void MarkupView::mousePressEvent(QMouseEvent *event)
{
    if (event->modifiers() == Qt::ShiftModifier)
        this->setDragMode(DragMode::ScrollHandDrag);
    else
        this->setDragMode(DragMode::NoDrag);

    if (deleteOption == false || invisibleSegmentOption == false)
        QGraphicsView::mousePressEvent(event);

    if (deleteOption == true || invisibleSegmentOption == true)
        viewport()->setCursor(Qt::PointingHandCursor);
    else if (event->modifiers() != Qt::ShiftModifier)
        viewport()->setCursor(Qt::CrossCursor);

    bool isLeftButton = (event->button() == Qt::LeftButton);
    bool isModifiers = (event->modifiers() == Qt::ShiftModifier);
    bool isClickedMark = clickOnLandmark(mapToScene(event->pos()), sizePointsValue*5);

    if (!isLeftButton || isModifiers || isClickedMark || deleteOption == true || allowEditOption == false || invisibleSegmentOption == true)
        return;

    QPointF position = mapToScene(event->pos());
    Landmark *landmark = createLandmark(position.x(), position.y(), sizePointsValue, sizePointsValue);
    addLandmark(landmark);
}



void MarkupView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    if (deleteOption == true || invisibleSegmentOption == true)
        viewport()->setCursor(Qt::PointingHandCursor);
    else if (event->modifiers() != Qt::ShiftModifier)
        viewport()->setCursor(Qt::CrossCursor);
}



void MarkupView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if (deleteOption == true || invisibleSegmentOption == true)
        viewport()->setCursor(Qt::PointingHandCursor);
    else if (event->modifiers() != Qt::ShiftModifier)
        viewport()->setCursor(Qt::CrossCursor);
}



void MarkupView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
    if (deleteOption == true || invisibleSegmentOption == true)
        viewport()->setCursor(Qt::PointingHandCursor);
    else if (event->modifiers() != Qt::ShiftModifier)
        viewport()->setCursor(Qt::CrossCursor);

    bool isLeftButton = (event->button() == Qt::LeftButton);
    bool isClickedMark = clickOnLandmark(mapToScene(event->pos()), sizePointsValue*5);

    if (!isLeftButton || !isClickedMark)
        return;

    if (deleteOption)
        deleteLandmark(mapToScene(event->pos()));

    if (invisibleSegmentOption)
        setInvisibleSegmentPoint(mapToScene(event->pos()));
}
