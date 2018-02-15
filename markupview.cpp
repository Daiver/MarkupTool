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

    QVector<QPointF> pointsF;
    int indEnd = 0;
    for (int indPoint = 0; indPoint < body->parts[body->indActived].points.size(); indPoint++){
        pointsF.push_back(body->parts[body->indActived].points[indPoint]->scenePos());
        if (body->parts[body->indActived].points[indPoint]->end == true){
            indEnd = indPoint;
            break;
        }
    }

    QPainterPath pathUp = buildSpline(QPolygonF(pointsF));

    body->parts[body->indActived].pathUp = new QGraphicsPathItem(pathUp);
    QPen pen(Qt::red);
    pen.setWidth(0.1);
    body->parts[body->indActived].pathUp->setPen(pen);
    scene->addItem(body->parts[body->indActived].pathUp);


    QVector<QPointF> pointsDown;
    for (int indPoint = indEnd; indPoint < body->parts[body->indActived].points.size(); indPoint++)
        pointsDown.push_back(body->parts[body->indActived].points[indPoint]->scenePos());
    pointsDown.push_back(body->parts[body->indActived].points.first()->scenePos());

    QPainterPath pathDown = buildSpline(QPolygonF(pointsDown));

    body->parts[body->indActived].pathDown = new QGraphicsPathItem(pathDown);
    QPen pen2(Qt::blue);
    pen2.setWidth(0.1);
    body->parts[body->indActived].pathDown->setPen(pen2);
    scene->addItem(body->parts[body->indActived].pathDown);

    /*
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
    */

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
        Landmark *landmark = new Landmark(newBody.parts[indPart].corner[indPoint].x()-0, newBody.parts[indPart].corner[indPoint].y()-0, 0.25, 0.25, indPoint);
        landmark->start = true;
        connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        body->parts[indPart].loadPoint(landmark, indPoint);

        for (int indPointUp = 0; indPointUp < newBody.parts[indPart].up.size(); indPointUp++){
            indPoint++;
            Landmark *landmark = new Landmark(newBody.parts[indPart].up[indPointUp].x()-0, newBody.parts[indPart].up[indPointUp].y()-0, 0.25, 0.25, indPoint);
            connect(landmark, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
            body->parts[indPart].loadPoint(landmark, indPoint);
        }

        indPoint++;
        Landmark *landmark2 = new Landmark(newBody.parts[indPart].corner[1].x()-0, newBody.parts[indPart].corner[1].y()-0, 0.25, 0.25, indPoint);
        landmark2->end = true;
        connect(landmark2, SIGNAL(changePosition(QPointF)), this, SLOT(updatePart()));
        body->parts[indPart].loadPoint(landmark2, indPoint);

        for (int indPointDown = 0; indPointDown < newBody.parts[indPart].down.size(); indPointDown++){
            indPoint++;
            Landmark *landmark = new Landmark(newBody.parts[indPart].down[indPointDown].x()-0, newBody.parts[indPart].down[indPointDown].y()-0, 0.25, 0.25, indPoint);
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

bool buildNaturalSpline( int size, double* t, double* p, double* a, double* b, double* c )
{
    int i;
    //  set up tridiagonal equation system; use coefficient
    //  vectors as temporary buffers
    double* h = new double[size-1];
    for (i = 0; i < size - 1; i++)
    {
        h[i] = t[i+1] - t[i];
        if (h[i] <= 0) {
            delete[] h;
            return false;
        }
    }

    double* d = new double[size-1];
    double dy1 = (p[1] - p[0]) / h[0];
    for (i = 1; i < size - 1; i++)
    {
        b[i] = c[i] = h[i];
        a[i] = 2.0 * (h[i-1] + h[i]);

        const double dy2 = (p[i+1] - p[i]) / h[i];
        d[i] = 6.0 * ( dy1 - dy2);
        dy1 = dy2;
    }

    //
    // solve it
    //

    // L-U Factorization
    for(i = 1; i < size - 2;i++)
    {
        c[i] /= a[i];
        a[i+1] -= b[i] * c[i];
    }

    // forward elimination
    double* s = new double[size];
    s[1] = d[1];
    for ( i = 2; i < size - 1; i++)
        s[i] = d[i] - c[i-1] * s[i-1];

    // backward elimination
    s[size - 2] = - s[size - 2] / a[size - 2];
    for (i = size -3; i > 0; i--)
        s[i] = - (s[i] + b[i] * s[i+1]) / a[i];
    s[size - 1] = s[0] = 0.0;

    //
    // Finally, determine the spline coefficients
    //
    for (i = 0; i < size - 1; i++)
    {
        a[i] = ( s[i+1] - s[i] ) / ( 6.0 * h[i]);
        b[i] = 0.5 * s[i];
        c[i] = ( p[i+1] - p[i] ) / h[i]
        - (s[i+1] + 2.0 * s[i] ) * h[i] / 6.0;
    }

    delete[] s;
    delete[] d;
    delete[] h;
    return true;
}

bool buildPeriodicSpline( int size, double* t, double* p, double* a, double* b, double* c )
{
    int i;

    //
    //  setup equation system; use coefficient
    //  vectors as temporary buffers
    //
    double* h = new double[size-1];
    for (i = 0; i < size - 1; i++)
    {
        h[i] = t[i+1] - t[i];
        if (h[i] <= 0) {
            delete[] h;
            return false;
        }
    }

    double* d = new double[size-1];
    const int imax = size - 2;
    double htmp = h[imax];
    double dy1 = (p[0] - p[imax]) / htmp;
    for (i = 0; i <= imax; i++)
    {
        b[i] = c[i] = h[i];
        a[i] = 2.0 * (htmp + h[i]);
        const double dy2 = (p[i+1] - p[i]) / h[i];
        d[i] = 6.0 * ( dy1 - dy2);
        dy1 = dy2;
        htmp = h[i];
    }

    //
    // solve it
    //

    // L-U Factorization
    a[0] = sqrt(a[0]);
    c[0] = h[imax] / a[0];
    double sum = 0;

    for( i = 0; i < imax - 1; i++)
    {
        b[i] /= a[i];
        if (i > 0)
            c[i] = - c[i-1] * b[i-1] / a[i];
        a[i+1] = sqrt( a[i+1] - b[i]*b[i] );
        sum += c[i]*c[i];
    }
    b[imax-1] = (b[imax-1] - c[imax-2] * b[imax-2]) / a[imax-1];
    a[imax] = sqrt(a[imax] - b[imax-1]*b[imax-1] - sum);


    // forward elimination
    double* s = new double[size];
    s[0] = d[0] / a[0];
    sum = 0;
    for( i = 1; i < imax; i++)
    {
        s[i] = (d[i] - b[i-1] * s[i-1]) / a[i];
        sum += c[i-1] * s[i-1];
    }
    s[imax] = (d[imax] - b[imax-1] * s[imax-1] - sum) / a[imax];


    // backward elimination
    s[imax] = - s[imax] / a[imax];
    s[imax-1] = -(s[imax-1] + b[imax-1] * s[imax]) / a[imax-1];
    for (i= imax - 2; i >= 0; i--)
        s[i] = - (s[i] + b[i] * s[i+1] + c[i] * s[imax]) / a[i];

    //
    // Finally, determine the spline coefficients
    //
    s[size-1] = s[0];
    for ( i=0; i < size-1; i++)
    {
        a[i] = ( s[i+1] - s[i] ) / ( 6.0 * h[i]);
        b[i] = 0.5 * s[i];
        c[i] = ( p[i+1] - p[i] )
            / h[i] - (s[i+1] + 2.0 * s[i] ) * h[i] / 6.0;
    }

    delete[] s;
    delete[] d;
    delete[] h;
    return true;
}

QPainterPath MarkupView::buildSpline(const QPolygonF &points)
{
    QPainterPath path;

    int i;
    int n = points.size();
    if ( n < 2 )
        return path;

    double* t = new double[n];

    double* px = new double[n];
    double* ax = new double[n];
    double* bx = new double[n];
    double* cx = new double[n];

    double* py = new double[n];
    double* ay = new double[n];
    double* by = new double[n];
    double* cy = new double[n];

    const QPointF* data = points.data();
    for (i=0; i < n; i++) {
        t[i] = i;
        px[i] = data[i].x();
        py[i] = data[i].y();
    }

    if (points.isClosed()) {
        buildPeriodicSpline( n, t, px, ax, bx, cx );
        buildPeriodicSpline( n, t, py, ay, by, cy );
    } else {
        buildNaturalSpline( n, t, px, ax, bx, cx );
        buildNaturalSpline( n, t, py, ay, by, cy );
    }

    path.moveTo( points[0] );
    for (i=0; i < n - 1; i++) {
        QPointF c1 = QPointF(px[i] + cx[i] / 3, py[i] + cy[i] / 3);
        QPointF c2 = QPointF(px[i] + cx[i] * 2/3 + bx[i] / 3, py[i] + cy[i] * 2/3 + by[i] / 3);
        path.cubicTo( c1, c2, points[i+1] );
    }

    delete[] cy;
    delete[] by;
    delete[] ay;
    delete[] py;

    delete[] cx;
    delete[] bx;
    delete[] ax;
    delete[] px;

    delete[] t;
    return path;
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
    Landmark *landmark = new Landmark(position.x(), position.y(), 0.5, 0.5, 0);
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
