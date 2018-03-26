#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "FileManager.h"
#include "ImageViewerWithSelector.h"

#include <QDialog>

#include <opencv2/core/core.hpp>

#include <QTreeWidgetItem>
#include <map>
#include <vector>

class ViewerWindow;

namespace Ui {
  class Navigator;
}


class Transformation
{
public:
    float zoom;
    int dx;
    int dy;

    Transformation(const Transformation &trans){
        zoom = trans.zoom;
        dx = trans.dx;
        dy = trans.dy;
    }

    Transformation(){
        zoom = 1;
        dx = 0;
        dy = 0;
    }

    Transformation(float zoom, int dx, int dy)
    {
        this->dx = dx;
        this->dy = dy;
        this->zoom = zoom;
    }
};

class InfoView
{
public:
    Transformation *tranformation;
    FileManager* fileManager;
    QTreeWidgetItem* parent;
    std::set <ImageViewerSelection*> *roiList;
};


class Navigator : public QDialog
{
    Q_OBJECT

  private:
  public:
    explicit Navigator(ViewerWindow *mainWindow, QWidget *parent = 0);
    ~Navigator();
    void addParentImage(FileManager *imageManager, const QString sourceName);

    QString imgePropertiesText(cv::Mat image);
    void addChildImage(cv::Mat image, const QString imageName, const bool changeToChild=true);
    void setViewer(ImageViewerWithSelector *viewer);
    void setImageNum(const int numImage);
    cv::Mat getImage();
    int getNumImages();
    void addPolygon(std::vector <cv::Point> polygon);
    FileManager* getFileManager();

    void storeCurrentTransformation();
    void setStoredTransformation(std::map<QTreeWidgetItem* , QMatrix >::const_iterator matrixIt, QTreeWidgetItem *item);
    void setStoredTransformation(QTreeWidgetItem *item);
private slots:
    void on_imageTree_itemSelectionChanged();

    void on_prevButton_clicked();

    void on_nextButton_clicked();

    void on_imageNumEdit_returnPressed();

    void on_firstButton_clicked();

    void on_lastButton_clicked();

    void on_imageNumSlider_valueChanged(int value);

private:
    Ui::Navigator *ui;

    std::map <QTreeWidgetItem* , InfoView  > infoViews;

    ImageViewerWithSelector *viewer;
    QTreeWidgetItem *lastItem;
    void evaluateButtonState();
    ViewerWindow *mainWindow;
    void addParentImage(FileManager *imageManager, const QString sourceName, Transformation *trans);
};

#endif // NAVIGATOR_H
