#include "ImageViewerPolygonSelection.h"
#include "Navigator.h"
#include "ui_Navigator.h"

#include "ViewerWindow.h"
#include <iostream>
#include <QScrollBar>

class SillyManager : public FileManager {
    cv::Mat image;
public:
    SillyManager(cv::Mat image) {this->image = image;}
    int getNumImages() {return 1;}
    cv::Mat getImage() {return image;}
    void goToImage(const int){}
    void nextImage(){}
    void prevImage(){}
    int getCurrentImageNumber() {return 0;}
};

Navigator::Navigator(ViewerWindow *mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Navigator)
{
    ui->setupUi(this);
    evaluateButtonState();
    this->mainWindow = mainWindow;

    ui->nextButton->setEnabled(false);
    ui->prevButton->setEnabled(false);
    ui->firstButton->setEnabled(false);
    ui->lastButton->setEnabled(false);
    ui->imageNumEdit->setEnabled(false);

    lastItem = NULL;
}

Navigator::~Navigator()
{
    delete ui;
}

QString Navigator::imgePropertiesText(cv::Mat image)
{
    QString properties = QString::number(image.cols)+" x "+QString::number(image.rows);
    properties += " x ";
    switch (image.type()) {
    case CV_32F:
    {
        properties += "float";
        break;
    }
    case CV_32S:
    {
        properties += "int";
        break;
    }
    case CV_8U:
    {
        properties += "byte";
        break;
    }
    case CV_8UC3:
    {
        properties += "3 bytes";
        break;
    }
    default:
        break;
    }
    return properties;
}

void Navigator::addParentImage(FileManager *imageManager, const QString sourceName, Transformation *trans)
{
    InfoView infoView;

    storeCurrentTransformation();
    QTreeWidgetItem *item = new QTreeWidgetItem();
    infoView.parent = item;

    item->setText(0,sourceName);
    QString properties = imgePropertiesText(imageManager->getImage());
    item->setText(1,properties);
    infoView.fileManager = imageManager;

    if (trans != NULL)
    {
        infoView.tranformation = trans;
    }
    else
    {
        const float MARGIN = 10;
        const float widthRatio = (float) viewer->width() / (float) (imageManager->getImage().cols + MARGIN);
        const float heightRatio = (float) viewer->height() / (float) (imageManager->getImage().rows + MARGIN);
        const float fitZoom = std::min(widthRatio,heightRatio);
        if (fitZoom > 1.0)
            infoView.tranformation = new Transformation(1,0,0);
        else
            infoView.tranformation = new Transformation(fitZoom,0,0);
    }

    infoView.roiList = new std::set <ImageViewerSelection*>();

    infoViews[item] = infoView;
    ui->imageTree->addTopLevelItem(item);
    ui->imageTree->setCurrentItem(item);
    evaluateButtonState();
}

void Navigator::addParentImage(FileManager *imageManager, const QString sourceName)
{
    addParentImage(imageManager, sourceName, NULL);
}

void Navigator::addChildImage(cv::Mat image, const QString imageName, const bool changeToChild)
{
    storeCurrentTransformation();

    QTreeWidgetItem *parentItem = ui->imageTree->currentItem();
    QTreeWidgetItem *item = new QTreeWidgetItem();

    //If parent image is navigable a copy is made
    InfoView parentInfo = infoViews[parentItem];
    FileManager *parentFileManager = parentInfo.fileManager;
    cv::Mat parentImage = parentFileManager->getImage();

    if (parentFileManager->getNumImages() > 1)
    {
        QString name = "Frame: " + QString::number(1+parentFileManager->getCurrentImageNumber());
        addParentImage(new SillyManager(parentImage.clone()),name, new Transformation(*parentInfo.tranformation));
        QTreeWidgetItem *newParentItem = ui->imageTree->currentItem();
        parentItem = newParentItem;
    }

    //If child image has same dimensions that parten image has the same transformation matrix
    InfoView infoView;
    if ((parentImage.rows == image.rows) && (parentImage.cols == image.cols))
        infoView.parent = parentInfo.parent;
    else
    {
        infoView.parent = item;
        const float MARGIN = 10;
        const float widthRatio = (float) viewer->width() / (float) (image.cols + MARGIN);
        const float heightRatio = (float) viewer->height() / (float) (image.rows + MARGIN);
        const float fitZoom = std::min(widthRatio,heightRatio);
        if (fitZoom > 1.0)
            infoView.tranformation = new Transformation(1,0,0);
        else
            infoView.tranformation = new Transformation(fitZoom,0,0);
    }

    infoView.roiList = new std::set <ImageViewerSelection*>();

    item->setText(0,imageName);
    QString properties = imgePropertiesText(image);
    item->setText(1,properties);

    infoView.fileManager = new SillyManager(image);
    infoViews[item]=infoView;

    parentItem->addChild(item);

    if (changeToChild)
    {
        ui->imageTree->setCurrentItem(item);

        setStoredTransformation(parentItem);
        evaluateButtonState();
    }
}

void Navigator::setViewer(ImageViewerWithSelector *viewer)
{
    this->viewer = viewer;
}

int Navigator::getNumImages()
{
    FileManager *manager = infoViews[ui->imageTree->currentItem()].fileManager;
    return manager->getNumImages();
}

void Navigator::addPolygon(std::vector<cv::Point> polygon)
{
    //InfoView &infoView = infoViews[ui->imageTree->currentItem()];

    /*
    QPolygonF qPolygon;
    for (int cont = 0; cont < polygon.size(); cont++)
    {
        qPolygon.push_back(QPointF(polygon[cont].x,polygon[cont].y));
    }

    QVector<QPolygonF> qPolygonVector;
    qPolygonVector.push_back(qPolygon);
    viewer->setPolygonList(qPolygonVector);
    */
/*
    ImageViewerPolygonSelection *polygonSelection = new ImageViewerPolygonSelection(qPolygon,viewer);
    infoView.roiList->insert(polygonSelection);
    viewer->repaint();*/
}

FileManager *Navigator::getFileManager()
{
    FileManager *manager = infoViews[ui->imageTree->currentItem()].fileManager;
    return manager;
}

cv::Mat Navigator::getImage()
{
    FileManager *manager = infoViews[ui->imageTree->currentItem()].fileManager;
    return manager->getImage();
}

void Navigator::setImageNum(const int numImage)
{
    storeCurrentTransformation();
    QTreeWidgetItem *item = ui->imageTree->currentItem();
    InfoView infoView = infoViews[item];
    FileManager *manager = infoView.fileManager;
    manager->goToImage(numImage);
    viewer->setImage(manager->getImage(),infoView.roiList);
    setStoredTransformation(item);
    evaluateButtonState();
}

void Navigator::evaluateButtonState()
{
    QTreeWidgetItem *item = ui->imageTree->currentItem();
    if (item != NULL)
    {
        FileManager *manager = infoViews[item].fileManager;
        const int numImages = manager->getNumImages();
        const int imageNumber = manager->getCurrentImageNumber();

        if (numImages < 2)
        {
            ui->nextButton->setEnabled(false);
            ui->prevButton->setEnabled(false);
            ui->firstButton->setEnabled(false);
            ui->lastButton->setEnabled(false);
        }
        else if (numImages > 1)
        {
            if (imageNumber > 0)
            {
                ui->prevButton->setEnabled(true);
                ui->firstButton->setEnabled(true);
            }
            else
            {
                ui->prevButton->setEnabled(false);
                ui->firstButton->setEnabled(false);
            }

            if (imageNumber < numImages-1)
            {
                ui->nextButton->setEnabled(true);
                ui->lastButton->setEnabled(true);
            }
            else
            {
                ui->nextButton->setEnabled(false);
                ui->lastButton->setEnabled(false);
            }
        }
        ui->imageNumSlider->setMaximum(numImages);
        ui->imageTotallabel->setText(QString::number(numImages));

        const int currentImageNumber = manager->getCurrentImageNumber()+1;
        ui->imageNumSlider->setValue(currentImageNumber);
        ui->imageNumEdit->setText(QString::number(currentImageNumber));
        ui->imageNumEdit->setEnabled(true);

        mainWindow->updateButtons();
    }
}


void Navigator::storeCurrentTransformation()
{
    if (lastItem != NULL)
    {
        const float zoom = viewer->transform().m11();
        const int dx = viewer->horizontalScrollBar()->value();
        const int dy = viewer->verticalScrollBar()->value();

        std::map <QTreeWidgetItem* , InfoView>::const_iterator infoViewIt = infoViews.find(infoViews[lastItem].parent);

        InfoView infoView = infoViewIt->second;
        Transformation *transf = infoView.tranformation;
        transf->zoom = zoom;
        transf->dx = dx;
        transf->dy = dy;
    }
}

void Navigator::setStoredTransformation(QTreeWidgetItem *item)
{
    std::map <QTreeWidgetItem* , InfoView>::const_iterator infoViewIt = infoViews.find(infoViews[item].parent);

    if (infoViewIt != infoViews.end())
    {
        InfoView infoView = infoViewIt->second;
        Transformation *transf = infoView.tranformation;
        viewer->resetTransform();
        viewer->scale(transf->zoom,transf->zoom);
        viewer->horizontalScrollBar()->setValue(transf->dx);
        viewer->verticalScrollBar()->setValue(transf->dy);
    }

    lastItem = ui->imageTree->currentItem();
}

void Navigator::on_imageTree_itemSelectionChanged()
{    
    storeCurrentTransformation();

    QTreeWidgetItem *item = ui->imageTree->currentItem();
    InfoView infoView = infoViews[item];
    FileManager *manager = infoView.fileManager;
    viewer->setImage(manager->getImage(),infoView.roiList);

    setStoredTransformation(item);
    evaluateButtonState();
}

void Navigator::on_prevButton_clicked()
{
    storeCurrentTransformation();
    QTreeWidgetItem *item = ui->imageTree->currentItem();
    InfoView infoView = infoViews[item];
    FileManager *manager = infoView.fileManager;
    manager->prevImage();
    viewer->setImage(manager->getImage(),infoView.roiList);
    setStoredTransformation(item);

    evaluateButtonState();
}

void Navigator::on_nextButton_clicked()
{
    storeCurrentTransformation();
    QTreeWidgetItem *item = ui->imageTree->currentItem();
    InfoView infoView = infoViews[item];
    FileManager *manager = infoView.fileManager;
    manager->nextImage();
    viewer->setImage(manager->getImage(),infoView.roiList);
    setStoredTransformation(item);

    evaluateButtonState();
}

void Navigator::on_imageNumEdit_returnPressed()
{
    storeCurrentTransformation();
    QTreeWidgetItem *item = ui->imageTree->currentItem();

    InfoView infoView = infoViews[item];
    FileManager *manager = infoView.fileManager;
    const int numImage = ui->imageNumEdit->text().toInt();
    if ((numImage > 0) && (numImage <= manager->getNumImages()))
    {
        manager->goToImage(numImage-1);
        viewer->setImage(manager->getImage(),infoView.roiList);
        setStoredTransformation(item);
        evaluateButtonState();
    }
}

void Navigator::on_firstButton_clicked()
{
    storeCurrentTransformation();
    QTreeWidgetItem *item = ui->imageTree->currentItem();
    InfoView infoView = infoViews[item];
    FileManager *manager = infoView.fileManager;
    manager->goToImage(0);
    viewer->setImage(manager->getImage(),infoView.roiList);
    setStoredTransformation(item);

    evaluateButtonState();
}

void Navigator::on_lastButton_clicked()
{
    storeCurrentTransformation();
    QTreeWidgetItem *item = ui->imageTree->currentItem();
    InfoView infoView = infoViews[item];
    FileManager *manager = infoView.fileManager;
    manager->goToImage(manager->getNumImages()-1);
    viewer->setImage(manager->getImage(),infoView.roiList);
    setStoredTransformation(item);

    evaluateButtonState();
}

void Navigator::on_imageNumSlider_valueChanged(int value)
{
    QTreeWidgetItem *item = ui->imageTree->currentItem();
    InfoView infoView = infoViews[item];
    FileManager *manager = infoView.fileManager;
    int sliderImageNumber = ui->imageNumSlider->value()-1;

    if (manager->getCurrentImageNumber() != sliderImageNumber)
    {
        storeCurrentTransformation();

        manager->goToImage(sliderImageNumber);
        viewer->setImage(manager->getImage(),infoView.roiList);
        setStoredTransformation(item);

        evaluateButtonState();
    }
}
