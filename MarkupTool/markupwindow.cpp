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
    QString dir = "/home/radiatus/Dataset/OutSource/Helen/";
    QString format = "Images (*.png *.jpg *.JPG)";

    QStringList imagesPaths = QFileDialog::getOpenFileNames(this, title, dir, format);
    images = new FilesPath(imagesPaths);
    shapes = new FilesPath(imagesPaths);
    shapes->setFormats("json");
    showImage(0);
}



bool MarkupWindow::isSizeContainsInd(const int &ind, const int &size) const
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
    ShapeFace body = LoaderJSON::getBody(doc);
    ui->markupView->setFaceShape(body);
    updateSearchedFiles(images->getNames());
}



void MarkupWindow::saveShape() const
{
    if (indOpenedImage == -1)
        return;

    QString path = shapes->getFilePath(indOpenedImage).getFullName();
    saveShape(path);
}



void MarkupWindow::saveShape(QString filePath) const
{
    QString path = filePath;
    ShapeFace *body = ui->markupView->getFaceShape();

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



void MarkupWindow::updateSearchedFiles(const QStringList &filesNames) const
{
    ui->searchedFiles->clear();
    ui->searchedFiles->addItems(filesNames);

    bool isCont = isSizeContainsInd(indOpenedImage, ui->searchedFiles->count());
    if(!isCont)
        return;

    QListWidgetItem *itemSelected = ui->searchedFiles->item(indOpenedImage);
    ui->searchedFiles->setItemSelected(itemSelected, true);

    int indReady = 0;
    for (int indImg = 0; indImg < ui->searchedFiles->count(); indImg++){
        QString path = shapes->getFilePath(indImg).dir + ui->searchedFiles->item(indImg)->text() + ".json";

        QFile file(path);
        if (!file.exists()){
            ui->searchedFiles->item(indImg)->setForeground(Qt::red);
            continue;
        }

        QJsonDocument doc = LoaderJSON::loadJson(path);
        ShapeFace body = LoaderJSON::getBody(doc);

        if (body.parts[indPart].corner.size() == 0){
            ui->searchedFiles->item(indImg)->setForeground(Qt::red);
            continue;
        }

        ui->searchedFiles->item(indImg)->setForeground(Qt::black);
        indReady++;

    }
    ui->ready->setText("Ready: " + QString::number(indReady));
}



void MarkupWindow::showImage(const int &indImage)
{
    bool isCont = isSizeContainsInd(indImage, images->size());
    if (!isCont)
        return;

    indOpenedImage = indImage;
    QString path = images->getFilePath(indImage).getFullName();
    QImageReader reader(path);
    reader.setAutoTransform(true);
    QImage image = reader.read();
    ui->markupView->drawImage(image);
    QString pathShape = shapes->getFilePath(indImage).getFullName();
    loadShape(pathShape);
    updateSearchedFiles(images->getNames());
}



void MarkupWindow::updateImage()
{
    bool isCont = isSizeContainsInd(indOpenedImage, images->size());
    if (!isCont)
        return;

    QString path = images->getFilePath(indOpenedImage).getFullName();
    QImageReader reader(path);
    reader.setAutoTransform(true);
    QImage image = reader.read();
    ui->markupView->updateImage(image);
    updateSearchedFiles(images->getNames());
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


void MarkupWindow::on_actionDelete_triggered(bool checked)
{
    ui->markupView->setDeleteOption(checked);
    ui->markupView->setInvisibleSegmentOption(false);
    ui->actioninvisible->setChecked(false);
}



void MarkupWindow::on_actionUpdate_from_JSON_triggered()
{
    if (indOpenedImage == -1)
        return;

    QString pathShape = shapes->getFilePath(indOpenedImage).getFullName();
    loadShape(pathShape);
}



void MarkupWindow::on_actionSave_shapeas_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save shape", "/home/radiatus/Dataset/", "*.json");
    saveShape(fileName+".json");
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
    for (int indNowShape = 0; indNowShape < shapes->size(); indNowShape++)
        for (int indShape = 0; indShape < shapesFiles.size(); indShape++){
            if (shapes->getFilePath(indNowShape).name == shapesPath.getFilePath(indShape).name){
                QString path = shapesFiles[indShape];
                shapes->filePath(indNowShape)->setPath(path);
                continue;
            }

            shapes->filePath(indNowShape)->dir = shapesPath.getFilePath(indShape).dir;
        }

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



void MarkupWindow::on_actioninvisible_triggered(bool checked)
{
    ui->markupView->setInvisibleSegmentOption(checked);
    ui->markupView->setDeleteOption(false);
    ui->actionDelete->setChecked(false);
}



void MarkupWindow::on_pointsSize_valueChanged(double value)
{
    ui->markupView->changeSizeLandmark(value);
}



void MarkupWindow::on_curve_activated(int index)
{
    ui->markupView->changeSegment(index);
    indPart = index;
    updateSearchedFiles(images->getNames());
}



void MarkupWindow::on_scaleOnSegment_valueChanged(int value)
{
    ui->markupView->setScaleParam(value);
}



void MarkupWindow::on_contrast_valueChanged(int value)
{
    if (value == ui->markupView->getContrast())
        return;
    ui->markupView->setContrast(value);
    updateImage();
}



void MarkupWindow::on_bright_valueChanged(int value)
{
    if (value == ui->markupView->getBright())
        return;
    ui->markupView->setBright(value);
    updateImage();
}


void MarkupWindow::on_scaleSave_toggled(bool value)
{
    ui->markupView->setScaleSave(value);
}



void MarkupWindow::on_allowEdit_toggled(bool value)
{
    ui->markupView->setAllowEdit(value);
}



void MarkupWindow::on_searchFile_textChanged(const QString &fileName)
{
    searchedFiles.clear();
    for (int indFiles = 0; indFiles < images->size(); indFiles++){
        QString fileName = images->getFilePath(indFiles).name;
        bool isContain = fileName.contains(fileName);
        if (isContain)
            searchedFiles.push_back(fileName);
    }

    updateSearchedFiles(searchedFiles);
}



void MarkupWindow::on_searchedFiles_itemDoubleClicked(QListWidgetItem *item)
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



void MarkupWindow::on_actionInvisibleSegment_triggered(bool checked)
{
    ui->markupView->setInvisibleSegmentOption(checked);
    ui->markupView->setDeleteOption(false);
    ui->actionDelete->setChecked(false);
}



void MarkupWindow::on_actionclearInvisibleSegment_triggered(bool checked)
{
    ui->markupView->getFaceShape()->getActivedPart()->clearInvisibleSegment();
    ui->markupView->updateSegmentPath();
    ui->markupView->updateSegmentPoints();
}



void MarkupWindow::on_actionNext_invisible_segment_triggered()
{
    ui->markupView->getFaceShape()->getActivedPart()->nextInvisibleSegment();
    ui->markupView->updateSegmentPath();
}



void MarkupWindow::on_actionPast_invisible_segment_triggered()
{
    ui->markupView->getFaceShape()->getActivedPart()->pastInvisibleSegment();
    ui->markupView->updateSegmentPath();
}
