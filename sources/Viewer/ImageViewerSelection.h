#ifndef IMAGEVIEWERSELECTION
#define IMAGEVIEWERSELECTION

#include <QPointF>


class ImageViewerSelection
{
protected:

    bool defining;

    bool selected;

public:

    ImageViewerSelection() {
        defining = false;
        selected = false;
    }

    virtual void endDefining() = 0;

    virtual bool isSelected(){return selected;}

    virtual bool isDefining(){return defining;}

    virtual void addPoint(const QPointF &p) = 0;

    virtual void moveCursor(const QPointF &p) = 0;

    virtual ~ImageViewerSelection(){}
};


class ImageViewerSelectionBuilder
{
public:
    virtual ImageViewerSelection* build() = 0;
};

#endif // IMAGEVIEWERSELECTION

