#ifndef MARKUPWINDOW_H
#define MARKUPWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImageReader>
#include <QListWidgetItem>
#include <QFile>
#include <QApplication>
#include <QClipboard>

#include "filespath.h"
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
    void updateListFiles(const QStringList &filesNames) const;
    void showImage(const int &indImage);

private slots:
    void on_loadFacesAction_triggered();
    void on_actionNext_triggered();
    void on_actionPast_triggered();
    void on_spinLandmarkSize_valueChanged(double arg1);
    void on_comboBoxBodyPart_activated(int index);
    void on_actionClear_triggered();
    void on_actionLoad_shape_triggered();
    void on_actionSave_shape_triggered();
    void on_listFiles_itemDoubleClicked(QListWidgetItem *item);
    void on_actionDelete_triggered();
    void on_actionDelete_triggered(bool checked);
    void on_actionUpdate_from_JSON_triggered();
    void on_actionSave_shapeas_triggered();
    void on_listFiles_itemClicked(QListWidgetItem *item);
    void on_lineEdit_textChanged(const QString &arg1);
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_sliderMoved(int position);
    void on_editCheak_stateChanged(int arg1);

    void on_scaleCheak_stateChanged(int arg1);

private:
    bool isContained(const int &ind, const int &size) const;
    void loadShape(QString path);
    void saveShape() const;
    void saveShape(QString filePath) const;

private:
    Ui::MarkupWindow *ui = nullptr;
    FilesPath *images = nullptr;
    QStringList openedInList;
    int indOpenedImage = -1;
    int indPart = 0;
};

#endif // MARKUPWINDOW_H
