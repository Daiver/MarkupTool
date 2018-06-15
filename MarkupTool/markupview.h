    #ifndef MARKUPVIEW_H
#define MARKUPVIEW_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QScrollBar>
#include <QGraphicsBlurEffect>

#include "body.h"
#include "QtMath"
#include "qmath.h"

#include "interpolation.h"
#include "splineadapter.h"

class MarkupView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MarkupView(QWidget *parent = 0);

    Landmark* createLandmark(qreal x, qreal y, qreal w, qreal h, int ind = 0, QPen pen = QPen(Qt::green), QBrush brush = QBrush(Qt::green));
    void drawImage(const QImage &image);
    bool clickOnLandmark(const QPointF &point, const float &radius) const;
    void addLandmark(Landmark *point, bool updatePath = true);
    void changeSizeLandmark(const double &size);
    void changeSegment(const int &indSegment);
    void updateImage(const QImage &image);
    void scaleOnSegment();
    void contrast(QImage &image);
    void bright(QImage &image);

    float getDistance(const QPointF &point, const QPointF &p1, const QPointF &p2) const;
    int getContrast() const;
    int getBright() const;

    ShapeFace* getFaceShape();
    QGraphicsScene* getScene();

    void setFaceShape(ShapeFace newFaceShape);
    void setScaleParam(const int &value);
    void setContrast(const int &value);
    void setBright(const int &value);
    void setAllowEdit(const bool &value);
    void setDeleteOption(const bool &value);
    void setScaleSave(const bool &value);
    void setInvisibleSegmentPoint(const QPointF &point);
    void setInvisibleSegmentOption(const bool &value);

    void centralOnSegment();
    void deleteLandmark(const QPointF &click);

    void clearScenePoints(const int &indPart);
    void clearScenePath(const int &indPart);
    void clearScene(const int &indPart);
    void clearPoints(const int &indPart);
    void clearPath(const int &indPart);
    void clear(const int &indPart);
    void clearAllPart(const int &indPart);
    void clearAll();

public slots:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void updateSegmentPath();
    void updateSegmentPoints();

private:
    QGraphicsScene *scene = nullptr;
    QGraphicsItem *imageItem = nullptr;
    QVector<QGraphicsPathItem*> paths;
    QImage image;

    int contrastValue = 0;
    int brightValue = 0;
    float scaleValue = 1.15;
    float scaleSegmentValue = 2;
    float sizePointsValue = 0.5;

    bool deleteOption = false;
    bool allowEditOption = true;
    bool invisibleSegmentOption = false;
    bool scaleSaveOption = true;

    ShapeFace *faceShape = new ShapeFace();
};

#endif // MARKUPVIEW_H
