#ifndef IMAGEVIEWERWITHSELECTOR_H
#define IMAGEVIEWERWITHSELECTOR_H

#include "ImageViewer.h"
#include <opencv2/core/mat.hpp>

class ImageViewerWithSelector : public ImageViewer
{
    ImageViewerSelectionBuilder *builder;
    ImageViewerSelection *definingItem;
    std::set <ImageViewerSelection*> *roiList;

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent( QMouseEvent * e);


public:
    ImageViewerWithSelector(QWidget *parent);
    ~ImageViewerWithSelector();

    virtual void setPolygonList(const std::vector<cv::Mat_<int>> &polygons);
    virtual void setPointList(const cv::Mat_<int> &points);
    virtual void setRectList(const cv::Mat_<int> &rects);

    virtual std::set<ImageViewerSelection *> *getRoiList() const;
    virtual void setRoiList(std::set<ImageViewerSelection *> *value);
    virtual void deleteCurrentPolygon();

    virtual std::vector<cv::Mat_<int> > getPolygonList() const;
    virtual cv::Mat_<int> getRectList() const;
    virtual cv::Mat_<int> getPointList() const;

    virtual void setSelectionBuilder(ImageViewerSelectionBuilder *builder);
    virtual void setImage(const cv::Mat &image, std::set<ImageViewerSelection *> *roiList);
};

#endif // IMAGEVIEWERWITHSELECTOR_H
