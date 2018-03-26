#ifndef IMAGEVIEWERPOINTSELECTION_H
#define IMAGEVIEWERPOINTSELECTION_H

#include "ImageViewerSelection.h"
#include <QGraphicsRectItem>

class ImageViewer;

class ImageViewerPointSelection : public QGraphicsRectItem, public ImageViewerSelection
{
    ImageViewer *viewer;

public:
    ImageViewerPointSelection(ImageViewer *viewer, QGraphicsItem * parent = 0 );

    ImageViewerPointSelection(const QPointF &p, ImageViewer *viewer, QGraphicsItem * parent = 0 );

    virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    virtual void addPoint(const QPointF &p);

    virtual void moveCursor(const QPointF &p);

    QPointF point() const;

    virtual ~ImageViewerPointSelection(){}

protected:

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    // ImageViewerSelection interface
public:
    virtual void endDefining();

    // QGraphicsItem interface
};


#endif // IMAGEVIEWERPOINTSELECTION_H

