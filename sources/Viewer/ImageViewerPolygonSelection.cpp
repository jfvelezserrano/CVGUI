#include "ImageViewerPolygonSelection.h"

#include <qpen.h>

#include <ImageViewer.h>

ImageViewerPolygonSelection::ImageViewerPolygonSelection(ImageViewer *viewer, QGraphicsItem *parent):QGraphicsPolygonItem(parent)
{
    this->viewer = viewer;
    defining = true;
}

ImageViewerPolygonSelection::ImageViewerPolygonSelection(const QPolygonF &polygon, ImageViewer *viewer, QGraphicsItem *parent):QGraphicsPolygonItem(parent)
{
    this->viewer = viewer;
    setPolygon(polygon);
}

void ImageViewerPolygonSelection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    QGraphicsPolygonItem::paint(painter,option,widget);
}

void ImageViewerPolygonSelection::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    selected = !selected;
    update();

    QGraphicsPolygonItem::mouseReleaseEvent(event);
}


void ImageViewerPolygonSelection::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //You need this to get mouseReleaseEnevts
    //QGraphicsRectItem::mousePressEvent(event);
}

void ImageViewerPolygonSelection::endDefining()
{
    QPolygonF polygon = this->polygon();
    if (polygon.size() > 0)
        polygon.remove(polygon.size()-1);
    setPolygon(polygon);

    defining = false;
    update();
}

void ImageViewerPolygonSelection::addPoint(const QPointF &p)
{
    QPolygonF polygon = this->polygon();
    if (polygon.size() > 0)
        polygon.remove(polygon.size()-1);

    polygon.append(p);
    polygon.append(p);
    setPolygon(polygon);
}

void ImageViewerPolygonSelection::moveCursor(const QPointF &p)
{
    QPolygonF polygon = this->polygon();

    if (polygon.size() > 0)
        polygon.remove(polygon.size()-1);

    polygon.append(p);
    setPolygon(polygon);
}

