#include "ImageViewerPointSelection.h"

#include <qpen.h>

#include <ImageViewer.h>

ImageViewerPointSelection::ImageViewerPointSelection(ImageViewer *viewer, QGraphicsItem *parent):QGraphicsRectItem(parent)
{
    this->viewer = viewer;
    defining = true;
}

ImageViewerPointSelection::ImageViewerPointSelection(const QPointF &p, ImageViewer *viewer, QGraphicsItem *parent):QGraphicsRectItem(parent)
{
    QRect rect((int)p.x(),(int)p.y(),1,1);
    setRect(rect);

    this->viewer = viewer;
}



void ImageViewerPointSelection::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    selected = !selected;
    update();

    QGraphicsRectItem::mouseReleaseEvent(event);
}


void ImageViewerPointSelection::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //You need this to get mouseReleaseEnevts
    //QGraphicsRectItem::mousePressEvent(event);
}

void ImageViewerPointSelection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen = this->pen();

    if (defining)
        pen.setColor(QColor(255,180,0));
    else if (selected)
        pen.setColor(QColor(255,0,0));
    else
        pen.setColor(QColor(0,180,0));

    QPolygonF polygon = viewer->mapToScene(0,0,4,4);
    const int w = std::max(std::abs(polygon[0].x()-polygon[1].x()),std::abs(polygon[0].y()-polygon[1].y()));
    pen.setWidth(w);
    setPen(pen);

    QGraphicsRectItem::paint(painter,option,widget);
}

void ImageViewerPointSelection::addPoint(const QPointF &p)
{
    QRect rect((int)p.x(),(int)p.y(),1,1);
    endDefining();
    setRect(rect);
}

void ImageViewerPointSelection::moveCursor(const QPointF &p)
{
}

QPointF ImageViewerPointSelection::point() const
{
    QRectF rect = this->rect();
    return QPointF(rect.x(),rect.y());
}


void ImageViewerPointSelection::endDefining()
{
    defining = false;
    update();

}
