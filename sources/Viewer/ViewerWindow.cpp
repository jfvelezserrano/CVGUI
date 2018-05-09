#include "ui_ViewerWindow.h"

#include "ImageFileManager.h"
#include "OperatorBuilderLoader.h"
#include "OperatorConfigurator.h"
#include "OperatorListener.h"
#include "VideoFileManager.h"
#include "ViewerWindow.h"
#include "Navigator.h"
#include "Histograms.h"
#include "ImageViewerPointSelection.h"
#include "ImageViewerPolygonSelection.h"
#include "ImageViewerRectangleSelection.h"

#include <QFileDialog>
#include <QSettings>

#include <Segmentation/SplitAndMerge.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

enum SelectorType {rectangle=0,polygon,point};

class MultiSelectorBuilder : public ImageViewerSelectionBuilder
{
    SelectorType selector = rectangle;
    ImageViewer *viewer;
public:

    MultiSelectorBuilder(ImageViewer *viewer) {
        this->viewer = viewer;
    }

    virtual ImageViewerSelection* build()
    {
        if (selector == rectangle)
            return new ImageViewerRectangleSelection(viewer);
        else if (selector ==polygon)
            return new ImageViewerPolygonSelection(viewer);
        else
            return new ImageViewerPointSelection(viewer);
    }

    void setSelector(SelectorType selector)
    {
        this->selector = selector;
    }
    int getSelector()
    {
        return selector;
    }

};

ViewerWindow::ViewerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewerWindow)
{
    ui->setupUi(this);

    ui->graphicsView->setInfoLabel(ui->coordsLabel,ui->valueLabel,ui->zoomLabel);
    ui->preview->setInfoLabel(ui->coordsLabel,ui->valueLabel,ui->zoomLabel);

    navigator = new Navigator(this);
    navigator->setViewer(ui->graphicsView);
    navigator->setWindowFlags(Qt::WindowStaysOnTopHint);
    navigator->show();

    logWindow = new LogWindow(this);

    histograms = new Histograms(NULL);
    histograms->setWindowFlags(Qt::WindowStaysOnTopHint);

    configurator = new ConfigurationDialog(navigator, this);

    imageListeners.push_back(histograms);
    imageListeners.push_back(configurator);

    selectionBuilder = new MultiSelectorBuilder(ui->graphicsView);
    ui->graphicsView->setSelectionBuilder(selectionBuilder);

    initOperators();

    QSettings settings("URJC-Gavab", "VisionViewer");
    lastLoadedFileName = settings.value("lastLoadedFileName").toString();
    lastLoadedSelectionFileName = settings.value("lastLoadedSelectionFileName").toString();
    lastSavedFileName = settings.value("lastSavedFileName").toString();
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    navigator->resize(settings.value("navigatorSize", QSize(500, 300)).toSize());
    navigator->move(settings.value("navigatorPos", QPoint(200, 200)).toPoint());
    configurator->resize(settings.value("configuratorSize", QSize(500, 300)).toSize());
    configurator->move(settings.value("configuratorPos", QPoint(200, 500)).toPoint());
}


void ViewerWindow::updateButtons()
{
    for (std::map <OperatorBuilder*,QAction *>::iterator it = actions.begin(); it != actions.end(); it++) {
        OperatorBuilder *op = it->first;
        QAction *ac = it->second;
        ac->setEnabled(op->isCompatible());
    }

    for (std::list <ImageListener*>::iterator it = imageListeners.begin(); it != imageListeners.end(); it++) {
        ImageListener *listener = *it;
        listener->updatePanelToImage(navigator->getImage());
    }
}


Navigator *ViewerWindow::getNavigator() const
{
    return navigator;
}

ConfigurationDialog *ViewerWindow::getConfigurator() const
{
    return configurator;
}

ImageViewerWithSelector *ViewerWindow::getViewer() const
{
    return ui->graphicsView;
}



void ViewerWindow::applyOperator(OperatorBuilder *o)
{
    startProcess();
    o->apply();
    endProcess();
}


void ViewerWindow::initOperators()
{
    OperatorBuilderLoader l;
    std::list <OperatorBuilder*> operators = l.getOperators();

    std::map<std::string, QMenu*> subMenus;

    for (std::list <OperatorBuilder*>::iterator b = operators.begin(); b != operators.end(); b++) {

        OperatorBuilder *op = *b;
        std::vector<std::string> description = op->description();
        op->setNavigator(navigator);
        OperatorConfigurator *conf = dynamic_cast <OperatorConfigurator*> (op);
        if (conf  != NULL)
        {
            conf->setPreviewViewer(ui->preview);
        }


        std::map<std::string, QMenu*>::const_iterator menu = subMenus.find(description[0]);
        if (menu == subMenus.end())
        {
            QMenu *subMenu = menuBar()->addMenu(description[0].c_str());
            subMenus[description[0]] = subMenu;
        }

        QMenu *selectedMenu = subMenus[description[0]];
        QAction *newAct = selectedMenu->addAction(description[description.size()-1].c_str());
        newAct->setEnabled(false);

        actions[op] = newAct;
        OperatorListener *listener = new OperatorListener(op,this);
        connect(newAct, SIGNAL(triggered()), listener, SLOT(action()));
    }
}


ViewerWindow::~ViewerWindow()
{
    delete ui;
}


FileManager * ViewerWindow::getManager(QStringList fileNames)
{
    FileManager *manager;

    const bool videoExtension = (fileNames.size() == 1) && (fileNames[0].endsWith("avi")||(fileNames[0].endsWith("mp4"))||(fileNames[0].endsWith("mpg")));

    if (videoExtension)
        manager = new VideoFileManager(fileNames);
    else
        manager = new ImageFileManager(fileNames);

    return manager;
}

LogWindow *ViewerWindow::getLogWindow()
{
    return logWindow;
}

void ViewerWindow::on_actionOpen_file_triggered()
{
    QString filter = "Images (*.ppm *.gif *.jpg *.png *.tif *.gif *.mat);;Videos (*.avi *.mp4 *.mpg);;All (*.*)";
    QString selectedFilter ="Images (*.ppm *.gif *.jpg *.png *.tif *.gif *.mat *.avi)";

    QStringList fileNames = QFileDialog::getOpenFileNames(this,"Open files",lastLoadedFileName,filter,&selectedFilter,QFileDialog::ReadOnly);

    if (fileNames.size() > 0)
    {
        FileManager *manager = getManager(fileNames);
        navigator->addParentImage(manager,fileNames[0]);

        lastLoadedFileName = fileNames[0];
        QSettings settings("URJC-Gavab", "VisionViewer");
        settings.setValue("lastLoadedFileName",lastLoadedFileName);
    }
}

void ViewerWindow::startProcess()
{
    setCursor(Qt::WaitCursor);
    time.start();
}

void ViewerWindow::endProcess()
{
    QString timeText = "Process time: " + QString::number(time.elapsed()/1000.0) + " (s)";
    ui->elapsedTime->setText(timeText);
    setCursor(Qt::ArrowCursor);
}

void ViewerWindow::on_actionSave_polygon_layer_triggered()
{
    QString filter = "Regions (*.yml);;All (*.*)";
    QString selectedFilter ="regions (*.yml)";

    QFileInfo info(navigator->getFileManager()->getCurrentImageName());
    QString fileName = info.absolutePath() + "/" + info.baseName()+".yml";
    fileName = QFileDialog::getSaveFileName(this,"Save regions file", fileName, filter, &selectedFilter);

    if (fileName != "")
    {
        cv::FileStorage fs(fileName.toStdString(), cv::FileStorage::WRITE);

        auto points = ui->graphicsView->getPointList();

        if (points.cols > 0)
            fs << "points" << points;

        auto rects = ui->graphicsView->getRectList();

        if (rects.cols > 0)
            fs << "rectangles" << rects;

        auto polygons = ui->graphicsView->getPolygonList();

        if (polygons.size() > 0)
            fs << "polygons" << polygons;

        fs.release();
    }
}

void ViewerWindow::on_actionLoad_polygon_layer_triggered()
{
    QString filter = "Polygons (*.yml);;All (*.*)";
    QString selectedFilter ="Polygons (*.yml)";
    lastLoadedSelectionFileName = QFileDialog::getOpenFileName(this,"Open region file",lastLoadedSelectionFileName,filter,&selectedFilter,QFileDialog::ReadOnly);
    if (lastLoadedSelectionFileName != "")
    {
        cv::FileStorage fs(lastLoadedSelectionFileName.toStdString(), cv::FileStorage::READ);
        cv::Mat_<int> points;
        cv::Mat_<int> rectangles;
        std::vector<cv::Mat_<int>> polygons;

        fs["points"] >> points;
        fs["rectangles"] >> rectangles;
        fs["polygons"] >> polygons;

        fs.release();

        ui->graphicsView->setPolygonList(polygons);
        ui->graphicsView->setPointList(points);
        ui->graphicsView->setRectList(rectangles);
    }
    QSettings settings("URJC-Gavab", "VisionViewer");
    settings.setValue("lastLoadedSelectionFileName",lastLoadedSelectionFileName);
}

void ViewerWindow::on_actionSave_Image_triggered()
{
    QString filter = "Images (*.gif *.jpg *.png *.tif *.mat);;All (*.*)";
    QString selectedFilter ="Images (*.gif *.jpg *.png *.tif *.mat)";

    lastSavedFileName = QFileDialog::getSaveFileName(this,"Save image file", lastSavedFileName, filter, &selectedFilter);

    if (lastSavedFileName != "")
    {
        if (lastSavedFileName.endsWith(".mat"))
        {
            std::vector <cv::Point> v;
            v.push_back(cv::Point(3,4));
            cv::FileStorage fs(lastSavedFileName.toStdString(), cv::FileStorage::WRITE);
            fs << "Mat" << ui->graphicsView->getImage();
            fs.release();
        }
        else
        {
            cv::imwrite(lastSavedFileName.toStdString(),ui->graphicsView->getImage());
        }
    }
    QSettings settings("URJC-Gavab", "VisionViewer");
    settings.setValue("lastSavedFileName",lastSavedFileName);
}



void ViewerWindow::on_actionLearnd_objects_triggered()
{
    //Select learning file

    //ObjectDetector(files)
}

void ViewerWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("URJC-Gavab", "VisionViewer");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("navigatorSize", navigator->size());
    settings.setValue("navigatorPos", navigator->pos());
    settings.setValue("configuratorSize", configurator->size());
    settings.setValue("configuratorPos", configurator->pos());
    QApplication::quit();
}


void ViewerWindow::on_actionHistogram_triggered()
{
    histograms->show();
}

void ViewerWindow::on_actionPolygonalSelection_triggered()
{
    ui->actionPointSelection->setChecked(false);
    ui->actionRectSelection->setChecked(false);
    selectionBuilder->setSelector(polygon);
}

void ViewerWindow::on_actionRectSelection_triggered()
{
    ui->actionPointSelection->setChecked(false);
    ui->actionPolygonalSelection->setChecked(false);
    selectionBuilder->setSelector(rectangle);
}

void ViewerWindow::on_actionPointSelection_triggered()
{
    ui->actionRectSelection->setChecked(false);
    ui->actionPolygonalSelection->setChecked(false);
    selectionBuilder->setSelector(point);
}

void ViewerWindow::on_actionZoom100_triggered()
{
    ui->graphicsView->resetTransform();
}

void ViewerWindow::on_actionZoomFit_triggered()
{
    ui->graphicsView->fitInView( ui->graphicsView->scene()->sceneRect(), Qt::KeepAspectRatio);
}


void ViewerWindow::on_actionLog_triggered()
{
    logWindow->show();
}

void ViewerWindow::on_tabWidget_currentChanged(int index)
{
    if (configurator->isVisible())
    {
        configurator->updatePanelToImage(navigator->getImage());
    }
}

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/flann/miniflann.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <climits>


bool isInPolygon(QVector<QPolygonF> &polygons,cv::KeyPoint &keypoint)
{
//    QPointF k(keypoint.pt.x,keypoint.pt.y);
//    for (QPolygonF &p : polygons)
//    {
//        if (p.containsPoint(k,Qt::WindingFill))
//            return true;
//    }
    return false;
}


int getDistance(const cv::Mat_<uchar> &row1,const cv::Mat_<uchar> &row2)
{
    int output = 0;
    for (int i = 0; i < row1.cols; i++)
    {
        output += __builtin_popcount(row1(0,i)^row2(0,i));
    }
    return output;
}

#include <iostream>

double regionSum(int x0, int y0, int x1, int y1, cv::Mat_<double> integralImage)
{
    const double w = integralImage(y0,x0);
    const double x = integralImage(y1,x1);
    const double y = integralImage(y0,x1);
    const double z = integralImage(y1,x0);

    return x - y - z + w;
}

void ViewerWindow::addHBorders(cv::Mat_<uchar> image, cv::Mat_<uchar> output)
{
    cv::Mat_<double> diference(image.rows,image.cols);
    diference.setTo(0);

    cv::Mat_<double> integralImage;
    cv::integral(image,integralImage,CV_64F);
    const int W = image.cols-1;

    const int TEXTURE_SIZE = 32;
    const int ROWS_IN_ANALYSIS = 1;

    const double DIVISOR = 255 * (TEXTURE_SIZE*2) * (ROWS_IN_ANALYSIS*2+1);

    for (int y = ROWS_IN_ANALYSIS; y < image.rows-ROWS_IN_ANALYSIS; y++)
        for (int x = 1; x < W; x++)
        {
            double leftMean  = regionSum(std::max(0,x-TEXTURE_SIZE),y-ROWS_IN_ANALYSIS,std::max(0,x-1),y+ROWS_IN_ANALYSIS+1,integralImage) / DIVISOR;
            double rightMean = regionSum(std::min(x+1,W),y-ROWS_IN_ANALYSIS,std::min(x+TEXTURE_SIZE,W),y+ROWS_IN_ANALYSIS+1,integralImage) / DIVISOR;
            diference(y,x) = fabs(leftMean- rightMean);
        }

    for (int y = ROWS_IN_ANALYSIS; y < image.rows-ROWS_IN_ANALYSIS; y++)
        for (int x = 1; x < W; x++)
        {

            if (diference(y,x) > 0.08)
            if ((diference(y,x-1) < diference(y,x)) && (diference(y,x) > diference(y,x+1)))
                output(y,x) = 255;
        }
}

void ViewerWindow::addVBorders(cv::Mat_<uchar> image, cv::Mat_<uchar> output)
{
    cv::Mat_<double> diference(image.rows,image.cols);
    diference.setTo(0);

    cv::Mat_<double> integralImage;
    cv::integral(image,integralImage,CV_64F);
    const int H = image.rows-1;

    const int TEXTURE_SIZE = 32;
    const int COLS_IN_ANALYSIS = 1;

    const double DIVISOR = 255 * (TEXTURE_SIZE*2) * (COLS_IN_ANALYSIS*2+1);

    for (int y = 1; y < image.rows-1; y++)
        for (int x = COLS_IN_ANALYSIS; x < image.cols-COLS_IN_ANALYSIS; x++)
        {
            double topMean  = regionSum(x-COLS_IN_ANALYSIS,std::max(0,y-TEXTURE_SIZE),x+COLS_IN_ANALYSIS+1,std::max(0,y-1),integralImage) / DIVISOR;
            double bottomMean = regionSum(x-COLS_IN_ANALYSIS,std::min(y+1,H),x+COLS_IN_ANALYSIS+1,std::min(y+TEXTURE_SIZE,H),integralImage) / DIVISOR;
            diference(y,x) = fabs(topMean- bottomMean);
        }

    for (int y = 1; y < image.rows-1; y++)
        for (int x = COLS_IN_ANALYSIS; x < image.cols-COLS_IN_ANALYSIS; x++)
        {

            if (diference(y,x) > 0.08)
            if ((diference(y-1,x) < diference(y,x)) && (diference(y,x) > diference(y+1,x)))
                output(y,x) = 255;
        }
}

