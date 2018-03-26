#ifndef IMAGEVIEWERPOLYGONSELECTION_H
#define IMAGEVIEWERPOLYGONSELECTION_H

#include "ImageViewerSelection.h"

#include <QGraphicsPolygonItem>

class ImageViewer;

class ImageViewerPolygonSelection : public QGraphicsPolygonItem, public ImageViewerSelection
{
    ImageViewer *viewer;

public:
    ImageViewerPolygonSelection(ImageViewer *viewer, QGraphicsItem * parent = 0 );

    ImageViewerPolygonSelection(const QPolygonF &polygon, ImageViewer *viewer, QGraphicsItem *parent=0);

    virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    virtual void endDefining();

    virtual void addPoint(const QPointF &p);

    virtual void moveCursor(const QPointF &p);

    virtual ~ImageViewerPolygonSelection(){}

protected:

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );


    // ImageViewerSelection interface

    // QGraphicsItem interface
};

#endif // IMAGEVIEWERPOLYGONSELECTION_H
