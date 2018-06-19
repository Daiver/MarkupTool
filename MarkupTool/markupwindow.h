#ifndef MARKUPWINDOW_H
#define MARKUPWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImageReader>
#include <QListWidgetItem>
#include <QFile>
#include <QApplication>
#include <QMessageBox>
#include <QClipboard>
#include "filespath.h"
#include "QElapsedTimer"
#include "loaderjsonbody.h"

namespace Ui {
class MarkupWindow;
}

class MarkupWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MarkupWindow(QWidget *parent = 0);
    ~MarkupWindow();

public:
    void updateSearchedFiles(const QStringList &filesNames) const;
    void showImage(const int &indImage);
    void updateImage();

private slots:
    void on_loadFacesAction_triggered();
    void on_actionNext_triggered();
    void on_actionPast_triggered();
    void on_actionClear_triggered();
    void on_actionLoad_shape_triggered();
    void on_actionSave_shape_triggered();
    void on_actionDelete_triggered(bool checked);
    void on_actionUpdate_from_JSON_triggered();
    void on_actionSave_shapeas_triggered();
    void on_actionLoad_shapes_as_triggered();
    void on_actionLoad_shapes_triggered();
    void on_actioninvisible_triggered(bool checked);
    void on_actionInvisibleSegment_triggered(bool checked);
    void on_actionclearInvisibleSegment_triggered(bool checked);
    void on_actionNext_invisible_segment_triggered();
    void on_actionPast_invisible_segment_triggered();

    void on_pointsSize_valueChanged(double value);
    void on_curve_activated(int index);
    void on_scaleOnSegment_valueChanged(int value);
    void on_contrast_valueChanged(int value);
    void on_bright_valueChanged(int value);
    void on_scaleSave_toggled(bool value);
    void on_allowEdit_toggled(bool value);
    void on_searchFile_textChanged(const QString &fileName);
    void on_searchedFiles_itemDoubleClicked(QListWidgetItem *item);

private:
    bool isSizeContainsInd(const int &ind, const int &size) const;
    void loadShape(QString path);
    void saveShape() const;
    void saveShape(QString filePath) const;

private:
    Ui::MarkupWindow *ui = nullptr;
    FilesPath *images = nullptr;
    FilesPath *shapes = nullptr;
    QStringList searchedFiles;
    bool onlyEmpty = false;
    int indOpenedImage = -1;
    int indPart = 0;
};

#endif // MARKUPWINDOW_H
