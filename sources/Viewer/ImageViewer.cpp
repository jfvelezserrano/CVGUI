#include "ImageViewer.h"

#include "RandomColorAsigner.h"
#include "ScaleColorAsigner.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QScrollBar>
#include <QGraphicsPolygonItem>
#include <QMouseEvent>

ImageViewer::ImageViewer(QWidget *parent) : QGraphicsView(parent)
{
    setMouseTracking(true);
    setDragMode(QGraphicsView::ScrollHandDrag);
    viewport()->setCursor(Qt::CrossCursor);
    pixmapItem = NULL;
    coordsLabel = NULL;
    valueLabel = NULL;
    zoomLabel = NULL;
}

void ImageViewer::wheelEvent(QWheelEvent *event)
{
    if (sourceImage.rows > 0)
    {
        QPointF center = this->mapToScene(event->x(),event->y());

        const int steps = event->delta() / 8 / 15;
        double jump;

        if (steps > 0)
            jump = powf(2.0,steps);
        else
            jump = powf(0.5,-steps);

        const double zoom = transform().m11();
        if ((zoom < 100) || (jump < 1))
        {
            scale(jump,jump);

            QPointF p2 = this->mapToScene(event->x(),event->y());
            QPointF desp1 = transform().m11() * (p2 - center);

            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - desp1.x() );
            verticalScrollBar()->setValue(verticalScrollBar()->value() - desp1.y());
        }
    }
}


cv::Mat ImageViewer::getImage() const
{
    return sourceImage.clone();
}


void ImageViewer::setImage(const cv::Mat &image)
{   
    //Remove old scene, but before recupere the items from it
    QGraphicsScene *oldScene = scene();
    delete oldScene;
    setScene(new QGraphicsScene(this));
    pixmapItem = NULL;

    if (image.rows > 0)
    {
        sourceImage = image.clone();

        //RGB image
        QPixmap qPixmap;
        if (image.channels() == 3)
        {
            cv::Mat_<cv::Vec3b> invertedChannels;
            cv::cvtColor(image,invertedChannels,CV_BGR2RGB);

            QImage qImg = QImage((const unsigned char*)(invertedChannels.data),invertedChannels.cols,invertedChannels.rows,invertedChannels.step,QImage::Format_RGB888);
            qPixmap = QPixmap::fromImage(qImg);
        }
        else if (image.channels() == 1)
        {
            if (image.type() == CV_8U)
            {
                QImage qImg = QImage((const unsigned char*)(image.data),image.cols,image.rows,image.step,QImage::Format_Indexed8);
                qPixmap = QPixmap::fromImage(qImg);
            }
            else if (image.type() == CV_32F)
            {
                cv::Mat_<cv::Vec3b> scaleColorImage = ScaleColorAsigner::assing((cv::Mat_<float>)image);
                QImage qImg = QImage((const unsigned char*)(scaleColorImage.data),scaleColorImage.cols,scaleColorImage.rows,scaleColorImage.step,QImage::Format_RGB888);
                qPixmap = QPixmap::fromImage(qImg);
            }
            else if (image.type() == CV_64F)
            {
                cv::Mat_<cv::Vec3b> scaleColorImage = ScaleColorAsigner::assing((cv::Mat_<double>)image);
                QImage qImg = QImage((const unsigned char*)(scaleColorImage.data),scaleColorImage.cols,scaleColorImage.rows,scaleColorImage.step,QImage::Format_RGB888);
                qPixmap = QPixmap::fromImage(qImg);
            }
            else if (image.type() == CV_32S)
            {
                cv::Mat_<cv::Vec3b> randomColorImage = RandomColorAsigner::assing(image);
                QImage qImg = QImage((const unsigned char*)(randomColorImage.data),randomColorImage.cols,randomColorImage.rows,randomColorImage.step,QImage::Format_RGB888);
                qPixmap = QPixmap::fromImage(qImg);
            }
        }
        if (qPixmap.height() > 0)
        {
            pixmapItem  = scene()->addPixmap(qPixmap);
        }

        setSceneRect(0,0,image.cols,image.rows);
    }
}


void ImageViewer::setInfoLabel(QLabel *coordsLabel, QLabel *valueLabel, QLabel *zoomLabel)
{
    this->coordsLabel = coordsLabel;
    this->valueLabel = valueLabel;
    this->zoomLabel = zoomLabel;
}


QString ImageViewer::getRGBComponentString(const cv::Vec3b &color) const
{
    return QString::number(color[2])+","+QString::number(color[1])+","+QString::number(color[0]);
}


QString ImageViewer::getHSVComponentString(const cv::Vec3b &color) const
{
    const float r = color[2];
    const float g = color[1];
    const float b = color[0];

    const float M = cv::max(r,cv::max(g,b));
    const float m = cv::min(r,cv::min(g,b));
    const float C = M - m;
    const float v = M;

    float hp;
    if (C == 0)
    {
        hp = 0;
    }
    else if (M == r)
    {
        const float aux = (g-b)/(C*6);
        hp = aux - (int)aux;
    }
    else if (M==g)
    {
        hp = 2.+(b-r)/C;
    }
    else
    {
        hp = 4.+(r-g)/C;
    }

    const int h = 30.0*hp;
    const int s = (C==0)?0:255*C/v;

    const QString output = QString::number(h) + "," + QString::number(s) + "," + QString::number(v);
    return output;
}


void ImageViewer::mouseMoveEvent( QMouseEvent * e)
{
    QGraphicsView::mouseMoveEvent(e);

    const QPointF p = mapToScene(e->x(),e->y());
    const int x = (int)p.x();
    const int y = (int)p.y();

    if ((sourceImage.rows > 0) && (x>0) && (y>0) && (x<sourceImage.cols) && (y<sourceImage.rows))
    {
        QString coords = "(" + QString::number(x) +" , "+ QString::number(y) + ")";

        if (coordsLabel != NULL)
            coordsLabel->setText(coords);

        if (sourceImage.channels() == 3)
        {
            cv::Vec3b color = sourceImage.at<cv::Vec3b>(y,x);

            QString  value = "RGB(" + getRGBComponentString(color) + ") HSV(" + getHSVComponentString(color) + ")";
            if (valueLabel != NULL)
                valueLabel->setText(value);
        }
        else
        {
            QString  value;
            switch (sourceImage.type())
            {
            case CV_8U:
            {
                QString grayStr = QString::number(sourceImage.at<uchar>(y,x));
                value = "Gray(" + grayStr + ")";
                break;
            }
            case CV_32F:
            {
                QString floatStr = QString::number(sourceImage.at<float>(y,x));
                value = "Float(" + floatStr + ")";
                break;
            }
            case CV_32S:
            {
                QString intStr = QString::number(sourceImage.at<int>(y,x));
                value = "Int(" + intStr + ")";
                break;
            }
            case CV_64F:
            {
                QString doubleStr = QString::number(sourceImage.at<double>(y,x));
                value = "Double(" + doubleStr + ")";
                break;
            }
            default:
                break;
            }
            if (valueLabel != NULL)
                valueLabel->setText(value);
        }
    }
}


void ImageViewer::drawGrid()
{
    const int height = scene()->height();
    const int width = scene()->width();

    for (int y = 0; y < sourceImage.rows; y++)
    {
        scene()->addLine(0,y,width,y);
    }

    for (int x = 0; x < sourceImage.cols; x++)
    {
        scene()->addLine(x,0,x,height);
    }

}

void ImageViewer::paintEvent(QPaintEvent *e)
{
    //If zoomed discontect antialias
    if (pixmapItem != NULL)
    {
        qreal currentScale = transform().m11();
        if ((currentScale < 1) /*&& ((sourceImage.type() == CV_8U) || (sourceImage.channels() == 3))*/)
            pixmapItem->setTransformationMode(Qt::SmoothTransformation);
        else
            pixmapItem->setTransformationMode(Qt::FastTransformation);

        QString zoomText = QString::number(transform().m11() * 100) + " %";

        if (zoomLabel != NULL)
            zoomLabel->setText(zoomText);

//        if (true)
//            drawGrid();
    }

    QGraphicsView::paintEvent(e);
}
