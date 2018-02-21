#ifndef MARKUPVIEW_H
#define MARKUPVIEW_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QWheelEvent>

#include "body.h"
#include "spline.h"

class MarkupView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MarkupView(QWidget *parent = 0);

    Landmark* createLandmark(qreal x, qreal y, qreal w, qreal h, int ind = 0, QPen pen = QPen(Qt::red), QBrush brush = QBrush(Qt::red));
    Body* getBody();
    void drawImage(const QImage &image);
    bool clickOnLandmark(const QPointF &point, const float &radius) const;
    void addLandmark(Landmark *point);
    void changeSizeLandmark(const double &size);
    void changeBodyPart(int indBlock);
    void setBody(Body newBody);
    QGraphicsScene* getScene();
    void setAllowEdit(bool allow);
    void setDeleteOption(bool option);
    void deleteLandmark(const QPointF &click);

    void clearScenePoints(int indPart);
    void clearScenePath(int indPart);
    void clearScene(int indPart);
    void clearPoints(int indPart);
    void clearPath(int indPart);
    void clear(int indPart);
    void clearAllPart(int indPart);
    void clearAll();

public slots:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void updateBodyPath();
    void updateBodyPoints();

private:
    QGraphicsScene *scene = nullptr;
    QGraphicsItem *image = nullptr;
    bool deleteOption = false;
    bool allowEdit = true;
    float sizeLandmark = 0.5;
    Body *body = new Body();
};

#endif // MARKUPVIEW_H
