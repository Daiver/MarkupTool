#include "markupwindow.h"
#include "ui_markupwindow.h"

MarkupWindow::MarkupWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MarkupWindow)
{
    ui->setupUi(this);
    images = new FilesPath();
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
    Body body = LoaderJSON::getBodyParts(doc);
    ui->markupView->setBody(body);
    updateListFiles();
}



void MarkupWindow::saveShape() const
{
    if (indOpenedImage == -1)
        return;

    FilePath imagePath = images->getFilePath(indOpenedImage);
    QString path = imagePath.dir + imagePath.name + ".json";

    Body *body = ui->markupView->getBody();

    QJsonDocument doc = LoaderJSON::createJson(body);
    LoaderJSON::saveJson(doc, path);
}



void MarkupWindow::updateListFiles() const
{
    QStringList filesNames = images->getNames();
    ui->listFiles->clear();
    ui->listFiles->addItems(filesNames);

    bool isCont = isContained(indOpenedImage, ui->listFiles->count());
    if(!isCont)
        return;

    QListWidgetItem *itemSelected = ui->listFiles->item(indOpenedImage);
    ui->listFiles->setItemSelected(itemSelected, true);

    int indReady = 0;
    for (int indImg = 0; indImg < images->size(); indImg++){
        QString path = images->getFilePath(indImg).dir + images->getFilePath(indImg).name + ".json";

        QFile file(path);
        if (!file.exists()){
            ui->listFiles->item(indImg)->setForeground(Qt::red);
            continue;
        }

        QJsonDocument doc = LoaderJSON::loadJson(path);
        Body body = LoaderJSON::getBodyParts(doc);

        if (body.parts[indPart].corner.size() == 0){
            ui->listFiles->item(indImg)->setForeground(Qt::red);
        }
        else{
            ui->listFiles->item(indImg)->setForeground(Qt::black);
            indReady++;
        }
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
    QString pathShape = images->getFilePath(indImage).dir + images->getFilePath(indImage).name + ".json";
    loadShape(pathShape);
    updateListFiles();
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
    updateListFiles();
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