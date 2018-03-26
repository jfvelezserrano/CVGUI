#include "FrameMorphologicalFilter.h"
#include "ui_FrameMorphologicalFilter.h"

#include "FrameMorphologicalFilter.h"
#include "MorphologicalFilterBuilder.h"

#include <QFileDialog>
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include <qgraphicsitem.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class QInteractiveGraphicsScene : public QGraphicsScene
{
private:
    QPixmap *qPixmap;
    QPainter *painter;
    QGraphicsView *gview;
    QGraphicsEllipseItem *elipse;
    cv::Mat_<uint8_t> image;
    MorphologyFilterBuilder *builder;

public:

    ~QInteractiveGraphicsScene()
    {
        delete qPixmap;
    }

    QInteractiveGraphicsScene(int w, int h, MorphologyFilterBuilder *builder, QGraphicsView *parent = Q_NULLPTR):QGraphicsScene(parent)
    {
        this->builder = builder;
        qPixmap = new QPixmap(w,h);
        qPixmap->fill();
        painter = new QPainter(qPixmap);
        QPen pen = painter->pen();
        pen.setColor(QColor(0,0,0));
        painter->setPen(pen);
        addPixmap(*qPixmap);

        image = cv::Mat_<uint8_t>(cv::Size(h,w),255);

        pen.setWidth(0);
        QBrush brush(Qt::SolidPattern);
        brush.setColor(QColor(255,0,0));
        elipse = addEllipse((w/2)+0.25,(h/2)+0.25,0.5,0.5,pen,brush);

        gview = parent;
    }

    QInteractiveGraphicsScene(cv::Mat_<uint8_t> image, MorphologyFilterBuilder *builder, QGraphicsView *parent = Q_NULLPTR):QGraphicsScene(parent)
    {
        this->builder = builder;
        QImage qImg = QImage((const unsigned char*)(image.data),image.cols,image.rows,image.step,QImage::Format_Indexed8);
        QPixmap qPixmap = QPixmap::fromImage(qImg);
        this->image = image;

        this->qPixmap = new QPixmap(qPixmap);
        const int w = this->qPixmap->width();
        const int h = this->qPixmap->height();

        painter = new QPainter(this->qPixmap);
        QPen pen = painter->pen();
        pen.setColor(QColor(0,0,0));
        painter->setPen(pen);

        addPixmap(*(this->qPixmap));

        pen.setWidth(0);
        QBrush brush(Qt::SolidPattern);
        brush.setColor(QColor(255,0,0));
        elipse = addEllipse((w/2)+0.25,(h/2)+0.25,0.5,0.5,pen,brush);

        gview = parent;
    }


    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
    {
        if (mouseEvent->button() == Qt::MouseButton::LeftButton)
        {
            QGraphicsScene::mousePressEvent(mouseEvent);
            int x = mouseEvent->scenePos().x();
            int y = mouseEvent->scenePos().y();

            if (image(y,x) == 0)
            {
                image(y,x) = 255;
                setPen(255);
            }
            else
            {
                image(y,x) = 0;
                setPen(0);
            }

            painter->drawPoint(x,y);
            addPixmap(*qPixmap);
            elipse->setZValue(1);
        }
        else
        {
            int x = mouseEvent->scenePos().x();
            int y = mouseEvent->scenePos().y();

            elipse->setRect(x+0.25,y+0.25,0.5,0.5);
        }
        builder->updatePreview();
    }

    void setPen(int value) {
        QPen pen = painter->pen();
        pen.setColor(QColor(value,value,value));
        painter->setPen(pen);
    }

    cv::Mat_<uint8_t> getImage()
    {
        return image;
    }
};

FrameMorphologicalFilter::FrameMorphologicalFilter(MorphologyFilterBuilder *builder, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameMorphologicalFilter)
{
    ui->setupUi(this);
    this->builder = builder;
    scene = new QInteractiveGraphicsScene(3,3,builder,ui->graphicsView_Element);
    ui->graphicsView_Element->setScene(scene);
}

FrameMorphologicalFilter::~FrameMorphologicalFilter()
{
    delete ui;
}


void FrameMorphologicalFilter::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    ui->graphicsView_Element->fitInView( ui->graphicsView_Element->scene()->sceneRect(), Qt::KeepAspectRatio);
}

void FrameMorphologicalFilter::setNewScene(QInteractiveGraphicsScene *newScene)
{
    QGraphicsScene *old = scene;
    scene = newScene;
    ui->graphicsView_Element->setScene(newScene);
    ui->graphicsView_Element->setScene(newScene);
    delete old;

    ui->graphicsView_Element->fitInView( ui->graphicsView_Element->scene()->sceneRect(), Qt::KeepAspectRatio);
    builder->updatePreview();
}


void FrameMorphologicalFilter::on_pushButton_newElement_clicked()
{    
    int shape;

    if (ui->comboBox_Type->currentIndex() == 0)
        shape = cv::MORPH_RECT;
    else if (ui->comboBox_Type->currentIndex() == 1)
        shape = cv::MORPH_CROSS;
    else
        shape = cv::MORPH_ELLIPSE;

    const int w = ui->spinBox_Width->value();
    const int h = ui->spinBox_Height->value();

    cv::Mat_<uint8_t> image = cv::getStructuringElement(shape,cv::Size2i(w,h));
    image = image * 255;

    setNewScene(new QInteractiveGraphicsScene(image,builder,ui->graphicsView_Element));
}

void FrameMorphologicalFilter::on_pushButton_LoadElement_clicked()
{
    QString filter = "Images (*.ppm *.gif *.jpg *.png *.tif *.gif *.mat);;All (*.*)";
    QString selectedFilter ="Images (*.ppm *.gif *.jpg *.png *.tif *.gif *.mat)";

    lastLoadedFileName = QFileDialog::getOpenFileName(this,"Open files",lastLoadedFileName,filter,&selectedFilter,QFileDialog::ReadOnly);

    if (lastLoadedFileName.size() > 0)
    {
        cv::Mat_<uint8_t> image = cv::imread(lastLoadedFileName.toStdString(),0);
        setNewScene(new QInteractiveGraphicsScene(image,builder,ui->graphicsView_Element));
    }
}

void FrameMorphologicalFilter::on_pushButton_SaveElement_clicked()
{
    QString filter = "Images (*.gif *.jpg *.png *.tif *.mat);;All (*.*)";
    QString selectedFilter ="Images (*.gif *.jpg *.png *.tif *.mat)";

    lastSavedFileName = QFileDialog::getSaveFileName(this,"Save image file", lastSavedFileName, filter, &selectedFilter);

    if (lastSavedFileName != "")
    {
        cv::imwrite(lastSavedFileName.toStdString(),scene->getImage());
    }    
}

cv::Mat_<uint8_t> FrameMorphologicalFilter::getElement()
{
    return scene->getImage();
}

bool FrameMorphologicalFilter::isDilate()
{
    return ui->radioButton_dilate->isChecked();
}


void FrameMorphologicalFilter::on_radioButton_dilate_toggled(bool checked)
{
    builder->updatePreview();
}
