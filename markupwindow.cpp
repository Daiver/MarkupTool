#include "markupwindow.h"
#include "ui_markupwindow.h"


MarkupWindow::MarkupWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MarkupWindow)
{
    ui->setupUi(this);
}


MarkupWindow::~MarkupWindow()
{
    delete ui;
}


void MarkupWindow::showImage(const int &indImage)
{
    clearAll();

    bool isEmptyImages = (imagesPaths.size() == 0);
    bool isUpOutside = (indImage >= imagesPaths.size());
    bool isDownOutside = (indImage < 0);

    if (isEmptyImages || isUpOutside || isDownOutside)
        return;

    indOpenedFile = indImage;
    QImageReader reader(imagesPaths[indImage]);
    reader.setAutoTransform(true);
    QImage image = reader.read();
    ui->graphicsView->drawImage(image);

    QListWidgetItem *itemSelected = ui->listWidget->item(indOpenedFile);
    ui->listWidget->setItemSelected(itemSelected, true);
}


void MarkupWindow::showImage(const QString &imageName)
{
    for (int indImage = 0; indImage < imagesPaths.size(); indImage++)
        if (imageName == imagesPaths[indImage])
            showImage(indImage);
}


void MarkupWindow::clearAll()
{
    ui->graphicsView->clearBodyParts();
}


void MarkupWindow::clear()
{
    ui->graphicsView->clearBodyPart();
}



void MarkupWindow::save()
{
    if (indOpenedFile == -1)
        return;
    QString imagePath = imagesPaths[indOpenedFile];

    Body body;
    for (int indPart = 0; indPart < body.parts.size(); indPart++){
        body.parts[indPart].corner = ui->graphicsView->getCorner(indPart);
        body.parts[indPart].up = ui->graphicsView->getUpPart(indPart);
        body.parts[indPart].down = ui->graphicsView->getDownPart(indPart);
    }

    QJsonDocument doc = LoaderJson::createJson(body);
    imagePath.remove(imagePath.size()-3, 3);
    imagePath += "json";
    LoaderJson::saveJson(doc, imagePath);
}



bool MarkupWindow::loadShape(QString path)
{
    QFile file(path);
    if (!file.exists())
        return false;

    QJsonDocument doc = LoaderJson::loadJson(path);
    Body body = LoaderJson::getBodyParts(doc);

    ui->graphicsView->updateBody(body);
    ui->graphicsView->changeBodyPart(indPart);
    updateList();
    return true;
}



void MarkupWindow::updateList()
{
    for (int ind = 0; ind < imagesPaths.size(); ind++){
        QString path = imagesPaths[ind];
        path.remove(path.size()-3, 3);
        path += "json";

        QFile file(path);
        if (!file.exists()){
            ui->listWidget->item(ind)->setForeground(Qt::red);
            continue;
        }
        QJsonDocument doc = LoaderJson::loadJson(path);
        Body body = LoaderJson::getBodyParts(doc);

        if (body.parts[ui->graphicsView->body->indActived].corner.size() == 0)
            ui->listWidget->item(ind)->setForeground(Qt::red);
        else
            ui->listWidget->item(ind)->setForeground(Qt::black);
    }
}


void MarkupWindow::on_actionLoad_images_triggered()
{
    imagesPaths = QFileDialog::getOpenFileNames(this, "load images", "/home/radiatus/Dataset/", "Images (*.png *.jpg)");
    ui->listWidget->clear();
    QStringList newImagePath;
    for (int indImg = 0; indImg < imagesPaths.size(); indImg++){
        QString path = imagesPaths[indImg];
        QStringList list = path.split("/");
        QString pathNew = list.last();
        newImagePath.push_back(pathNew);
    }
    ui->listWidget->addItems(newImagePath   );

    updateList();

    showImage(0);
    QString path = imagesPaths[0];
    path.remove(path.size()-3, 3);
    path += "json";
    loadShape(path);
}


void MarkupWindow::on_actionNext_triggered()
{
    save();
    showImage(indOpenedFile+1);
    if (indOpenedFile == -1)
        return;
    QString path = imagesPaths[indOpenedFile];
    path.remove(path.size()-3, 3);
    path += "json";
    loadShape(path);
}


void MarkupWindow::on_actionPast_triggered()
{
    save();
    showImage(indOpenedFile-1);
    if (indOpenedFile == -1)
        return;
    QString path = imagesPaths[indOpenedFile];
    path.remove(path.size()-3, 3);
    path += "json";
    loadShape(path);
}


void MarkupWindow::on_actionClear_all_triggered()
{
    clearAll();
}


void MarkupWindow::on_actionSave_shape_triggered()
{
    save();
}


void MarkupWindow::on_actionSave_JSON_triggered()
{
    QString pathFile = QFileDialog::getOpenFileName(this, "loadFiles", "/home/radiatus/Dataset/", "*.json");
    loadShape(pathFile);
}


void MarkupWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    save();
    QString imageName = item->text();

    for (int indImg = 0; indImg < imagesPaths.size(); indImg++){
        QString path = imagesPaths[indImg];
        QStringList list = path.split("/");
        QString last = list.last();
        if (imageName == last){
            imageName = path;
            break;
        }
    }

    showImage(imageName);
    if (indOpenedFile == -1)
        return;
    QString path = imagesPaths[indOpenedFile];
    path.remove(path.size()-3, 3);
    path += "json";
    loadShape(path);

}


void MarkupWindow::on_actionLeft_Eye_triggered()
{
    indPart = 0;
    ui->graphicsView->changeBodyPart(indPart);
    updateList();
}


void MarkupWindow::on_actionRight_Eye_triggered()
{
    indPart = 1;
    ui->graphicsView->changeBodyPart(indPart);
    updateList();
}


void MarkupWindow::on_actionMouth_triggered()
{
    indPart = 2;
    ui->graphicsView->changeBodyPart(indPart);
    updateList();
}


void MarkupWindow::on_actionClear_triggered()
{
    clear();
}

void MarkupWindow::on_actionCount_ready_triggered()
{
    int count = 0;
    for (int ind = 0; ind < imagesPaths.size(); ind++){
        QString path = imagesPaths[ind];
        path.remove(path.size()-3, 3);
        path += "json";

        QFile file(path);
        if (!file.exists())
            continue;

        QJsonDocument doc = LoaderJson::loadJson(path);
        Body body = LoaderJson::getBodyParts(doc);

        if (body.parts[ui->graphicsView->body->indActived].corner.size() != 0)
            count++;
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("Count ready");
    msgBox.setText(QString::number(count));
    msgBox.exec();
}
