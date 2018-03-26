#ifndef VISIONGRAPHICSVIEW_H
#define VISIONGRAPHICSVIEW_H

#include "ImageViewerSelection.h"

#include <QGraphicsView>
#include <QLabel>
#include <QPolygonF>

#include <opencv2/core/core.hpp>

#include <vector>
#include <set>


class ImageViewer : public QGraphicsView
{
    QLabel* coordsLabel;
    QLabel* valueLabel;
    QLabel* zoomLabel;
    QGraphicsPixmapItem *pixmapItem;

protected:
    cv::Mat sourceImage;
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *e);

public:
    ImageViewer(QWidget *parent);
    virtual void setImage(const cv::Mat &image);
    void setInfoLabel(QLabel* coordsLabel, QLabel *valueLabel, QLabel *zoomLabel);
    cv::Mat getImage() const;

    void drawGrid();
private:
    QString getRGBComponentString(const cv::Vec3b &color) const;
    QString getHSVComponentString(const cv::Vec3b &color) const;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);
};

#endif // VISIONGRAPHICSVIEW_H
