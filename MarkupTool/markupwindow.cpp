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
    QElapsedTimer timer;
    timer.start();
    QString title = "load images";
    QString dir = "/home/radiatus/Dataset/OutSource/Helen/";
    QString format = "Images (*.png *.jpg *.JPG)";

    QStringList imagesPaths = QFileDialog::getOpenFileNames(this, title, dir, format);
    images = new FilesPath(imagesPaths);
    shapes = new FilesPath(imagesPaths);
    shapes->setFormats("json");
    showImage(0);
    qDebug() << "on_loadFacesAction_triggered " + QString::number(timer.elapsed()/100);
}

bool MarkupWindow::isSizeContainsInd(const int &ind, const int &size) const
{
    QElapsedTimer timer;
    timer.start();
    bool isEmptyList = (size == 0);
    bool isUpOutside = (ind >= size);
    bool isDownOutside = (ind < 0);

    qDebug() << "isSizeContainsInd " + QString::number(timer.elapsed()/100);
    if (isEmptyList || isUpOutside || isDownOutside)
        return false;

    return true;
}

void MarkupWindow::loadShape(QString path)
{
    QElapsedTimer timer;
    timer.start();
    QFile file(path);
    if (!file.exists())
        return;

    QJsonDocument doc = LoaderJSON::loadJson(path);
    ShapeFace body = LoaderJSON::getBody(doc);
    ui->markupView->setFaceShape(body);
    updateSearchedFiles(images->getNames());
    qDebug() << "loadShape " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::saveShape() const
{
    QElapsedTimer timer;
    timer.start();
    if (indOpenedImage == -1){
        qDebug() << "saveShape " + QString::number(timer.elapsed()/100);
        return;
    }

    QString path = shapes->getFilePath(indOpenedImage).getFullName();
    saveShape(path);
    qDebug() << "saveShape " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::saveShape(QString filePath) const
{
    QElapsedTimer timer;
    timer.start();
    QString path = filePath;
    ShapeFace *body = ui->markupView->getFaceShape();

    QFile file(filePath);
    if (file.exists() && body->isEmpty()){
        file.remove();
        qDebug() << "saveShape string " + QString::number(timer.elapsed()/100);
        return;
    }

    if (body->isEmpty()){
        qDebug() << "saveShape string " + QString::number(timer.elapsed()/100);
        return;
    }

    QJsonDocument doc = LoaderJSON::createJson(body);
    LoaderJSON::saveJson(doc, path);
    qDebug() << "saveShape string " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::updateSearchedFiles(const QStringList &filesNames) const
{
    QElapsedTimer timer;
    timer.start();

    ui->searchedFiles->clear();
    ui->searchedFiles->addItems(filesNames);

    bool isCont = isSizeContainsInd(indOpenedImage, ui->searchedFiles->count());
    if(!isCont){
        qDebug() << "updateSearchedFiles " + QString::number(timer.elapsed()/100);
        return;
    }

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
    qDebug() << "updateSearchedFiles " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::showImage(const int &indImage)
{
    QElapsedTimer timer;
    timer.start();
    bool isCont = isSizeContainsInd(indImage, images->size());
    if (!isCont){
        qDebug() << "showImage " + QString::number(timer.elapsed()/100);
        return;
    }

    indOpenedImage = indImage;
    QString path = images->getFilePath(indImage).getFullName();
    QImageReader reader(path);
    reader.setAutoTransform(true);
    QImage image = reader.read();
    ui->markupView->drawImage(image);
    QString pathShape = shapes->getFilePath(indImage).getFullName();
    loadShape(pathShape);
    updateSearchedFiles(images->getNames());
    qDebug() << "showImage " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::updateImage()
{
    QElapsedTimer timer;
    timer.start();
    bool isCont = isSizeContainsInd(indOpenedImage, images->size());
    if (!isCont){
        qDebug() << "updateImage " + QString::number(timer.elapsed()/100);
        return;
    }

    QString path = images->getFilePath(indOpenedImage).getFullName();
    QImageReader reader(path);
    reader.setAutoTransform(true);
    QImage image = reader.read();
    ui->markupView->updateImage(image);
    updateSearchedFiles(images->getNames());
    qDebug() << "updateImage " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionNext_triggered()
{
    QElapsedTimer timer;
    timer.start();
    saveShape();
    showImage(indOpenedImage+1);
    qDebug() << "on_actionNext_triggered " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionPast_triggered()
{
    QElapsedTimer timer;
    timer.start();
    saveShape();
    showImage(indOpenedImage-1);
    qDebug() << "on_actionPast_triggered " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionClear_triggered()
{
    QElapsedTimer timer;
    timer.start();
    ui->markupView->clearAllPart(indPart);
    qDebug() << "on_actionClear_triggered " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionLoad_shape_triggered()
{
    QElapsedTimer timer;
    timer.start();
    QString pathFile = QFileDialog::getOpenFileName(this, "loadFiles", "/home/radiatus/Dataset/", "*.json");
    loadShape(pathFile);
    qDebug() << "on_actionLoad_shape_triggered " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionSave_shape_triggered()
{
    QElapsedTimer timer;
    timer.start();
    saveShape();
    qDebug() << "on_actionSave_shape_triggered " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionDelete_triggered(bool checked)
{
    QElapsedTimer timer;
    timer.start();
    ui->markupView->setDeleteOption(checked);
    ui->markupView->setInvisibleSegmentOption(false);
    ui->actioninvisible->setChecked(false);
    qDebug() << "on_actionDelete_triggered " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionUpdate_from_JSON_triggered()
{
    QElapsedTimer timer;
    timer.start();
    if (indOpenedImage == -1){
        qDebug() << "on_actionUpdate_from_JSON_triggered " + QString::number(timer.elapsed()/100);
        return;
    }

    QString pathShape = shapes->getFilePath(indOpenedImage).getFullName();
    loadShape(pathShape);
    qDebug() << "on_actionUpdate_from_JSON_triggered " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionSave_shapeas_triggered()
{
    QElapsedTimer timer;
    timer.start();
    QString fileName = QFileDialog::getSaveFileName(this, "Save shape", "/home/radiatus/Dataset/", "*.json");
    saveShape(fileName+".json");
     qDebug() << "on_actionSave_shapeas_triggered " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionLoad_shapes_as_triggered()
{
    QElapsedTimer timer;
    timer.start();
    if (indOpenedImage == -1){
        qDebug() << "on_actionLoad_shapes_as_triggered " + QString::number(timer.elapsed()/100);
        return;
    }

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
    qDebug() << "on_actionLoad_shapes_as_triggered " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionLoad_shapes_triggered()
{
    QElapsedTimer timer;
    timer.start();
    if (indOpenedImage == -1){
        qDebug() << "on_actionLoad_shapes_triggered " + QString::number(timer.elapsed()/100);
        return;
    }

    shapes = new FilesPath(images->getFullNames());
    shapes->setFormats("json");
    showImage(indOpenedImage);
}

void MarkupWindow::on_actioninvisible_triggered(bool checked)
{
    QElapsedTimer timer;
    timer.start();
    ui->markupView->setInvisibleSegmentOption(checked);
    ui->markupView->setDeleteOption(false);
    ui->actionDelete->setChecked(false);
    qDebug() << "on_actioninvisible_triggered " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_pointsSize_valueChanged(double value)
{
    QElapsedTimer timer;
    timer.start();
    ui->markupView->changeSizeLandmark(value);
    qDebug() << "on_pointsSize_valueChanged " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_curve_activated(int index)
{
    QElapsedTimer timer;
    timer.start();
    ui->markupView->changeSegment(index);
    indPart = index;
    updateSearchedFiles(images->getNames());
    qDebug() << "on_curve_activated " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_scaleOnSegment_valueChanged(int value)
{
    QElapsedTimer timer;
    timer.start();
    ui->markupView->setScaleParam(value);
     qDebug() << "on_scaleOnSegment_valueChanged " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_contrast_valueChanged(int value)
{
    QElapsedTimer timer;
    timer.start();
    if (value == ui->markupView->getContrast()){
        qDebug() << "on_contrast_valueChanged " + QString::number(timer.elapsed()/100);
        return;
    }
    ui->markupView->setContrast(value);
    updateImage();
    qDebug() << "on_contrast_valueChanged " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_bright_valueChanged(int value)
{
    QElapsedTimer timer;
    timer.start();
    if (value == ui->markupView->getBright()){
        qDebug() << "on_bright_valueChanged " + QString::number(timer.elapsed()/100);
        return;
    }
    ui->markupView->setBright(value);
    updateImage();
    qDebug() << "on_bright_valueChanged " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_scaleSave_toggled(bool value)
{
    QElapsedTimer timer;
    timer.start();
    ui->markupView->setScaleSave(value);
}

void MarkupWindow::on_allowEdit_toggled(bool value)
{
    QElapsedTimer timer;
    timer.start();
    ui->markupView->setAllowEdit(value);
    qDebug() << "on_allowEdit_toggled " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_searchFile_textChanged(const QString &fileName)
{
    QElapsedTimer timer;
    timer.start();
    searchedFiles.clear();
    for (int indFiles = 0; indFiles < images->size(); indFiles++){
        QString fileName = images->getFilePath(indFiles).name;
        bool isContain = fileName.contains(fileName);
        if (isContain)
            searchedFiles.push_back(fileName);
    }

    updateSearchedFiles(searchedFiles);
    qDebug() << "on_searchFile_textChanged " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_searchedFiles_itemDoubleClicked(QListWidgetItem *item)
{
    QElapsedTimer timer;
    timer.start();
    //QApplication::clipboard()->deleteLater();
    //QApplication::clipboard()->setText(item->text());
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
    qDebug() << "on_searchedFiles_itemDoubleClicked " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionInvisibleSegment_triggered(bool checked)
{
    QElapsedTimer timer;
    timer.start();
    ui->markupView->setInvisibleSegmentOption(checked);
    ui->markupView->setDeleteOption(false);
    ui->actionDelete->setChecked(false);
    qDebug() << "on_actionInvisibleSegment_triggered " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionclearInvisibleSegment_triggered(bool checked)
{
    QElapsedTimer timer;
    timer.start();
    ui->markupView->getFaceShape()->getActivedPart()->clearInvisibleSegment();
    ui->markupView->updateSegmentPath();
    ui->markupView->updateSegmentPoints();
    qDebug() << "on_actionclearInvisibleSegment_triggered " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionNext_invisible_segment_triggered()
{
    QElapsedTimer timer;
    timer.start();
    ui->markupView->getFaceShape()->getActivedPart()->nextInvisibleSegment();
    ui->markupView->updateSegmentPath();
    qDebug() << "on_actionNext_invisible_segment_triggered " + QString::number(timer.elapsed()/100);
}

void MarkupWindow::on_actionPast_invisible_segment_triggered()
{
    QElapsedTimer timer;
    timer.start();
    ui->markupView->getFaceShape()->getActivedPart()->pastInvisibleSegment();
    ui->markupView->updateSegmentPath();
    qDebug() << "on_actionPast_invisible_segment_triggered " + QString::number(timer.elapsed()/100);
}
