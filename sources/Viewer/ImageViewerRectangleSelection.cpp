#include "ImageViewerRectangleSelection.h"
#include <QGraphicsSceneMouseEvent>
#include <ImageViewer.h>

ImageViewerRectangleSelection::ImageViewerRectangleSelection(ImageViewer *viewer, QGraphicsItem *parent):QGraphicsRectItem(parent)
{
    this->viewer = viewer;
    defining = true;
}

ImageViewerRectangleSelection::ImageViewerRectangleSelection(const QRectF &rect, ImageViewer *viewer, QGraphicsItem *parent):QGraphicsRectItem(parent)
{
    this->viewer = viewer;
    setRect(rect);
}


void ImageViewerRectangleSelection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void ImageViewerRectangleSelection::endDefining()
{
    defining = false;
    update();
}

void ImageViewerRectangleSelection::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //You need this to get mouseReleaseEnevts
    //QGraphicsRectItem::mousePressEvent(event);

    if (!defining)
    {
        QPointF p = event->pos();

        QPointF a = p - rect().bottomRight();
        QPointF b = p - rect().topLeft();

        if ((std::abs(a.x()) < 10) && (std::abs(a.y()) < 10))
        {
            defining = true;
        }
        else if ((std::abs(b.x()) < 10) && (std::abs(b.y()) < 10))
        {
            defining = true;
        }
    }
}

void ImageViewerRectangleSelection::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    selected = !selected;
    update();

    QGraphicsRectItem::mouseReleaseEvent(event);
}


void ImageViewerRectangleSelection::addPoint(const QPointF &p)
{
    QRectF rect = this->rect();

    if (rect.isEmpty())
    {
        startpoint = p;
        rect.setTopLeft(p.toPoint());
        rect.setBottomRight(p.toPoint());
    }
    else
    {
        int minX = std::min(p.x(),startpoint.x());
        int minY = std::min(p.y(),startpoint.y());
        int maxX = std::max(p.x(),startpoint.x());
        int maxY = std::max(p.y(),startpoint.y());
        rect.setCoords(minX,minY,maxX,maxY);
        //rect.setBottomRight(p.toPoint());
        endDefining();
    }

    setRect(rect);
}

void ImageViewerRectangleSelection::moveCursor(const QPointF &p)
{
    QRectF rect = this->rect();
    int minX = std::min(p.x(),startpoint.x());
    int minY = std::min(p.y(),startpoint.y());
    int maxX = std::max(p.x(),startpoint.x());
    int maxY = std::max(p.y(),startpoint.y());
    rect.setCoords(minX,minY,maxX,maxY);
    setRect(rect);
}

