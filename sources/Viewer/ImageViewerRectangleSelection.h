#ifndef IMAGEVIEWERRECTANGLESELECTION_H
#define IMAGEVIEWERRECTANGLESELECTION_H

#include "ImageViewerSelection.h"

#include <QGraphicsRectItem>

class ImageViewer;

class ImageViewerRectangleSelection : public QGraphicsRectItem, public ImageViewerSelection
{
    ImageViewer *viewer;
    QPointF startpoint;

public:
    ImageViewerRectangleSelection(ImageViewer *viewer, QGraphicsItem * parent = 0 );

    ImageViewerRectangleSelection(const QRectF &rect, ImageViewer *viewer, QGraphicsItem *parent=0);

    virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    virtual void endDefining();

    virtual void addPoint(const QPointF &p);

    virtual void moveCursor(const QPointF &p);

    virtual ~ImageViewerRectangleSelection(){}

protected:

    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );

    // ImageViewerSelection interface

    // QGraphicsItem interface
};

#endif // IMAGEVIEWERRECTANGLESELECTION_H
