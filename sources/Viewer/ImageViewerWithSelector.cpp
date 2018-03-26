#include "ImageViewerWithSelector.h"

#include "ImageViewerPointSelection.h"
#include "ImageViewerPolygonSelection.h"
#include "ImageViewerRectangleSelection.h"
#include <QMouseEvent>

ImageViewerWithSelector::ImageViewerWithSelector(QWidget *parent):ImageViewer(parent)
{
    definingItem = NULL;
    roiList = NULL;
}

ImageViewerWithSelector::~ImageViewerWithSelector()
{

}

std::set<ImageViewerSelection *> *ImageViewerWithSelector::getRoiList() const
{
    return roiList;
}

void ImageViewerWithSelector::setRoiList(std::set<ImageViewerSelection *> *value)
{
    roiList = value;
}

void ImageViewerWithSelector::mouseMoveEvent( QMouseEvent * e)
{
    ImageViewer::mouseMoveEvent(e);

    if (definingItem != NULL)
    {
        const QPointF p = mapToScene(e->x(),e->y());
        definingItem->moveCursor(p);
    }
}

void ImageViewerWithSelector::setPolygonList(const std::vector<cv::Mat_<int>> &polygons)
{
    for (int cont = 0; cont < polygons.size(); cont++)
    {
        QPolygonF qpolygon;

        for (int col = 0; col < polygons[cont].cols; col++)
        {
            QPointF qpoint(polygons[cont](0,col),polygons[cont](1,col));
            qpolygon.append(qpoint);
        }

        ImageViewerPolygonSelection *polygonItem = new ImageViewerPolygonSelection(qpolygon, this);
        scene()->addItem(polygonItem);
        roiList->insert(polygonItem);
    }
}


void ImageViewerWithSelector::setPointList(const cv::Mat_<int> &points)
{
    for (int col = 0; col < points.cols; col++)
    {
        QPointF qpoint(points(0,col),points(1,col));
        ImageViewerPointSelection *pointItem = new ImageViewerPointSelection(qpoint, this);
        scene()->addItem(pointItem);
        roiList->insert(pointItem);
    }
}


void ImageViewerWithSelector::setRectList(const cv::Mat_<int> &rects)
{
    for (int col = 0; col < rects.cols; col++)
    {
        QRectF qrect(rects(0,col),rects(1,col),rects(2,col),rects(3,col));
        ImageViewerRectangleSelection *rectItem = new ImageViewerRectangleSelection(qrect,this);
        scene()->addItem(rectItem);
        roiList->insert(rectItem);
    }
}

std::vector<cv::Mat_<int>> ImageViewerWithSelector::getPolygonList() const
{
    std::vector<cv::Mat_<int>> output;

    for (std::set <ImageViewerSelection*>::iterator it = roiList->begin(); it != roiList->end(); it++)
    {
        ImageViewerSelection *selection = *it;

        ImageViewerPolygonSelection *polygonSelection = dynamic_cast <ImageViewerPolygonSelection*>(selection);
        if (polygonSelection != NULL)
        {
            QPolygonF qpolygon = polygonSelection->polygon();

            cv::Mat_<int> polygon(2,qpolygon.size());

            for (int cont = 0; cont < qpolygon.size(); cont++)
            {
                polygon(0,cont) = qpolygon.at(cont).x();
                polygon(1,cont) = qpolygon.at(cont).y();
            }

            output.push_back(polygon);
        }
    }
    return output;
}

cv::Mat_<int> ImageViewerWithSelector::getRectList() const
{
    std::vector<QRectF> rectangles;
    for (std::set <ImageViewerSelection*>::iterator it = roiList->begin(); it != roiList->end(); it++)
    {
        ImageViewerSelection *selection = *it;

        ImageViewerRectangleSelection *rectSelection = dynamic_cast <ImageViewerRectangleSelection*>(selection);
        if (rectSelection != NULL)
        {
            rectangles.push_back(rectSelection->rect());
        }
    }

    cv::Mat_<int> output(4,rectangles.size());

    for (int cont = 0; cont < rectangles.size(); cont++)
    {
        output(0,cont) = rectangles[cont].left();
        output(1,cont) = rectangles[cont].top();
        output(2,cont) = rectangles[cont].width();
        output(3,cont) = rectangles[cont].height();
    }

    return output;
}

cv::Mat_<int> ImageViewerWithSelector::getPointList() const
{
    std::vector <QPointF> points;
    for (std::set <ImageViewerSelection*>::iterator it = roiList->begin(); it != roiList->end(); it++)
    {
        ImageViewerSelection *selection = *it;

        ImageViewerPointSelection *pointSelection = dynamic_cast <ImageViewerPointSelection*>(selection);
        if (pointSelection != NULL)
        {
            points.push_back(pointSelection->point());
        }
    }

    cv::Mat_<int> output(2,points.size());

    for (int cont = 0; cont < points.size(); cont++)
    {
        output(0,cont) = points[cont].x();
        output(1,cont) = points[cont].y();
    }

    return output;
}



void ImageViewerWithSelector::setSelectionBuilder(ImageViewerSelectionBuilder *builder)
{
    this->builder = builder;
}

void ImageViewerWithSelector::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if (definingItem != NULL)
        {
            definingItem->endDefining();
            roiList->insert(definingItem);
            definingItem = NULL;
        }
    }
    else if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Escape)
    {
        if (definingItem != NULL)
            deleteCurrentPolygon();

        std::list <ImageViewerSelection*> eraseList;

        if (roiList != NULL)
        {
            for (std::set <ImageViewerSelection*>::iterator it = roiList->begin(); it != roiList->end(); it++)
            {
                ImageViewerSelection *graphicPolygon = *it;
                if (graphicPolygon->isSelected())
                {
                    eraseList.push_back(graphicPolygon);
                }
            }

            for (std::list <ImageViewerSelection*>::iterator it = eraseList.begin(); it != eraseList.end(); it++)
            {
                ImageViewerSelection *graphicPolygon = *it;
                roiList->erase(graphicPolygon);
                QGraphicsItem *item = dynamic_cast< QGraphicsItem *>(graphicPolygon);
                scene()->removeItem(item);
            }

        }
    }
}

void ImageViewerWithSelector::setImage(const cv::Mat &image, std::set <ImageViewerSelection*> *roiList)
{
    QGraphicsScene *oldScene = scene();

    if (this->roiList != NULL)
    {
        for (std::set <ImageViewerSelection*>::iterator it = this->roiList->begin(); it != this->roiList->end(); it++)
        {
            QGraphicsItem *selection = dynamic_cast<QGraphicsItem *>(*it);
            oldScene->removeItem(selection);
        }
    }

    ImageViewer::setImage(image);

    deleteCurrentPolygon();

    if (roiList == NULL)
        this->roiList = new std::set <ImageViewerSelection*>();
    else
    {
        this->roiList = roiList;

        for (std::set <ImageViewerSelection*>::iterator it = roiList->begin(); it != roiList->end(); it++)
        {
            QGraphicsItem *selection = dynamic_cast<QGraphicsItem *>(*it);
            scene()->addItem(selection);
        }
    }

}

void ImageViewerWithSelector::deleteCurrentPolygon()
{
    if (definingItem != NULL)
    {
        QGraphicsItem *item = dynamic_cast< QGraphicsItem *>(definingItem);
        scene()->removeItem(item);
        delete definingItem;
        definingItem = NULL;
    }
}


void ImageViewerWithSelector::mouseReleaseEvent(QMouseEvent *event)
{
    ImageViewer::mouseReleaseEvent(event);

    if ((event->button() == Qt::RightButton) && (sourceImage.rows > 0))
    {
        const QPointF p = mapToScene(event->x(),event->y());

        if (definingItem == NULL)
        {
            definingItem = builder->build();
            QGraphicsScene *scene = this->scene();
            QGraphicsItem *item = dynamic_cast< QGraphicsItem *>(definingItem);
            scene->addItem(item);
        }
        definingItem->addPoint(p);
        if (!definingItem->isDefining())
        {
            roiList->insert(definingItem);
            definingItem = NULL;
        }
    }

//Sacar de la lista el roi
    //if ((event->button() == Qt::LeftButton) && (sourceImage.rows > 0))
//    {
//        if algun roi esta en definingItem
//        defining = roiList

//    }

    viewport()->setCursor(Qt::CrossCursor);
}

