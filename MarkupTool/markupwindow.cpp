#include "markupwindow.h"
#include "ui_markupwindow.h"

MarkupWindow::MarkupWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MarkupWindow)
{
    ui->setupUi(this);
    images = new FilesPath();
    shapes = new FilesPath();
}



MarkupWindow::~MarkupWindow()
{
    delete ui;
}



void MarkupWindow::on_loadFacesAction_triggered()
{
    QString title = "load images";
    QString dir = "/home/radiatus/Dataset/";
    QString format = "Images (*.png *.jpg)";

    QStringList imagesPaths = QFileDialog::getOpenFileNames(this, title, dir, format);
    images = new FilesPath(imagesPaths);
    shapes = new FilesPath(imagesPaths);
    shapes->setFormats("json");
    showImage(0);
}



bool MarkupWindow::isContained(const int &ind, const int &size) const
{
    bool isEmptyList = (size == 0);
    bool isUpOutside = (ind >= size);
    bool isDownOutside = (ind < 0);

    if (isEmptyList || isUpOutside || isDownOutside)
        return false;

    return true;
}



void MarkupWindow::loadShape(QString path)
{
    QFile file(path);
    if (!file.exists())
        return;

    QJsonDocument doc = LoaderJSON::loadJson(path);
    Body body = LoaderJSON::getBody(doc);
    ui->markupView->setBody(body);
    updateListFiles(images->getNames());
}



void MarkupWindow::saveShape() const
{
    if (indOpenedImage == -1)
        return;

    QString path = shapes->getFilePath(indOpenedImage).fullName();
    saveShape(path);
}



void MarkupWindow::saveShape(QString filePath) const
{
    QString path = filePath;
    Body *body = ui->markupView->getBody();

    QFile file(filePath);
    if (file.exists() && body->isEmpty()){
        file.remove();
        return;
    }

    if (body->isEmpty())
        return;

    QJsonDocument doc = LoaderJSON::createJson(body);
    LoaderJSON::saveJson(doc, path);
}



void MarkupWindow::updateListFiles(const QStringList &filesNames) const
{
    ui->listFiles->clear();
    ui->listFiles->addItems(filesNames);

    bool isCont = isContained(indOpenedImage, ui->listFiles->count());
    if(!isCont)
        return;

    QListWidgetItem *itemSelected = ui->listFiles->item(indOpenedImage);
    ui->listFiles->setItemSelected(itemSelected, true);

    int indReady = 0;
    for (int indImg = 0; indImg < ui->listFiles->count(); indImg++){
        QString path = shapes->getFilePath(indImg).dir + ui->listFiles->item(indImg)->text() + ".json";

        QFile file(path);
        if (!file.exists()){
            ui->listFiles->item(indImg)->setForeground(Qt::red);
            continue;
        }

        QJsonDocument doc = LoaderJSON::loadJson(path);
        Body body = LoaderJSON::getBody(doc);

        if (body.parts[indPart].corner.size() == 0){
            ui->listFiles->item(indImg)->setForeground(Qt::red);
            continue;
        }

        ui->listFiles->item(indImg)->setForeground(Qt::black);
        indReady++;

    }
    ui->ready->setText("Ready: " + QString::number(indReady));
}



void MarkupWindow::showImage(const int &indImage)
{
    bool isCont = isContained(indImage, images->size());
    if (!isCont)
        return;

    indOpenedImage = indImage;
    QString path = images->getFilePath(indImage).fullName();
    QImageReader reader(path);
    reader.setAutoTransform(true);
    QImage image = reader.read();
    ui->markupView->drawImage(image);
    QString pathShape = shapes->getFilePath(indImage).fullName();
    loadShape(pathShape);
    updateListFiles(images->getNames());
}



void MarkupWindow::on_actionNext_triggered()
{
    saveShape();
    showImage(indOpenedImage+1);
}



void MarkupWindow::on_actionPast_triggered()
{
    qDebug() << indOpenedImage;
    saveShape();
    showImage(indOpenedImage-1);
}



void MarkupWindow::on_spinLandmarkSize_valueChanged(double arg1)
{
    ui->markupView->changeSizeLandmark(arg1);
}



void MarkupWindow::on_comboBoxBodyPart_activated(int index)
{
    ui->markupView->changeBodyPart(index);
    indPart = index;
    updateListFiles(images->getNames());
}



void MarkupWindow::on_actionClear_triggered()
{
    ui->markupView->clearAllPart(indPart);
}



void MarkupWindow::on_actionLoad_shape_triggered()
{
    QString pathFile = QFileDialog::getOpenFileName(this, "loadFiles", "/home/radiatus/Dataset/", "*.json");
    loadShape(pathFile);
}



void MarkupWindow::on_actionSave_shape_triggered()
{
    saveShape();
}



void MarkupWindow::on_listFiles_itemDoubleClicked(QListWidgetItem *item)
{
    QApplication::clipboard()->setText(item->text());
    saveShape();
    FilePath imagePath;
    for (int indImg = 0; indImg < images->size(); indImg++){
        QString name = images->getFilePath(indImg).name;
        if (name != item->text())
            continue;
        indOpenedImage = indImg;
        imagePath = images->getFilePath(indImg);
    }

    showImage(indOpenedImage);
}



void MarkupWindow::on_actionDelete_triggered()
{

}


void MarkupWindow::on_actionDelete_triggered(bool checked)
{
    ui->markupView->setDeleteOption(checked);
}



void MarkupWindow::on_actionUpdate_from_JSON_triggered()
{
    if (indOpenedImage == -1)
        return;

    QString pathShape = shapes->getFilePath(indOpenedImage).fullName();
    loadShape(pathShape);
}



void MarkupWindow::on_actionSave_shapeas_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save shape", "/home/radiatus/Dataset/", "*.json");
    saveShape(fileName+".json");
}



void MarkupWindow::on_listFiles_itemClicked(QListWidgetItem *item)
{

}



void MarkupWindow::on_lineEdit_textChanged(const QString &arg1)
{
    openedInList.clear();
    for (int indFiles = 0; indFiles < images->size(); indFiles++){
        QString fileName = images->getFilePath(indFiles).name;
        bool isContain = fileName.contains(arg1);
        if (isContain)
            openedInList.push_back(fileName);
    }

    updateListFiles(openedInList);
}

void MarkupWindow::on_horizontalSlider_valueChanged(int value)
{

}


void MarkupWindow::on_horizontalSlider_sliderMoved(int position)
{
    ui->markupView->setScaleParam(position);
}



void MarkupWindow::on_editCheak_stateChanged(int arg1)
{
     ui->markupView->setAllowEdit(bool(arg1));
}



void MarkupWindow::on_scaleCheak_stateChanged(int arg1)
{
    ui->markupView->setScaleSave(bool(arg1));
}



void MarkupWindow::on_actionLoad_shapes_as_triggered()
{
    if (indOpenedImage == -1)
        return;

    QString title = "load shapes";
    QString dir = "/home/radiatus/Dataset/";
    QString format = "*.json";

    QStringList shapesFiles = QFileDialog::getOpenFileNames(this, title, dir, format);

    FilesPath shapesPath(shapesFiles);
    for (int indShape = 0; indShape < shapesFiles.size(); indShape++)
        for (int indNowShape = 0; indNowShape < shapes->size(); indNowShape++)
            if (shapesPath.getFilePath(indShape).name == shapes->getFilePath(indNowShape).name){
                QString path = shapesFiles[indShape];
                shapes->filePath(indNowShape)->setPath(path);
            }


    //FilesPath *f = shapes;
    showImage(indOpenedImage);
}



void MarkupWindow::on_actionLoad_shapes_triggered()
{
    if (indOpenedImage == -1)
        return;

    shapes = new FilesPath(images->getFullNames());
    shapes->setFormats("json");
    showImage(indOpenedImage);
}
