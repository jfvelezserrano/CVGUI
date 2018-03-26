#include "FrameGaborFilter.h"
#include "GaborFilterBuilder.h"
#include "ScaleColorAsigner.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "ui_FrameGaborFilter.h"

#include <QGraphicsItem>

FrameGaborFilter::FrameGaborFilter(GaborFilterBuilder *builder, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGaborFilter)
{
    ui->setupUi(this);
    this->builder = builder;
    ui->graphicsView->setScene(new QGraphicsScene(this));
}

FrameGaborFilter::~FrameGaborFilter()
{
    delete ui;
}

void FrameGaborFilter::on_thetaSlider_valueChanged(int value)
{
    ui->thetaValue->setText(QString::number(value));
    theta = value * CV_PI / 180.0;
    updateMatrix();
}

void FrameGaborFilter::on_psiSlider_valueChanged(int value)
{
    ui->psiValue->setText(QString::number(value));
    psi = value * CV_PI / 180.0;
    updateMatrix();
}

double FrameGaborFilter::getKernelRadious() const
{
    return kernelRadious;
}

double FrameGaborFilter::getGamma() const
{
    return gamma;
}

double FrameGaborFilter::getPsi() const
{
    return psi;
}

double FrameGaborFilter::getLambda() const
{
    return lambda;
}

double FrameGaborFilter::getTheta() const
{
    return theta;
}

double FrameGaborFilter::getSigma() const
{
    return sigma;
}

double FrameGaborFilter::updateMatrix() const
{
    cv::Size_<double> ksize(kernelRadious*2+1,kernelRadious*2+1);
    cv::Mat_<double> kernel = cv::getGaborKernel(ksize, sigma, theta, lambda, gamma, psi);

    cv::Mat_<cv::Vec3b> scaleColorImage = ScaleColorAsigner::assing((cv::Mat_<double>)kernel);
    QImage qImg = QImage((const unsigned char*)(scaleColorImage.data),scaleColorImage.cols,scaleColorImage.rows,scaleColorImage.step,QImage::Format_RGB888);
    ui->graphicsView->scene()->clear();
    QGraphicsPixmapItem *pixmapItem = ui->graphicsView->scene()->addPixmap(QPixmap::fromImage(qImg));

    pixmapItem->setTransformationMode(Qt::FastTransformation);
    ui->graphicsView->setSceneRect(0,0,kernel.cols,kernel.rows);
    ui->graphicsView->fitInView(0,0,kernel.cols,kernel.rows,Qt::KeepAspectRatio);

    builder->updatePreview();
}


void FrameGaborFilter::on_gammaValue_valueChanged(double arg1)
{
    gamma = arg1;
    updateMatrix();
}

void FrameGaborFilter::on_sigmaValue_valueChanged(double arg1)
{
    sigma = arg1;
    updateMatrix();
}

void FrameGaborFilter::on_kernelRadious_valueChanged(int arg1)
{
    kernelRadious = arg1;
    updateMatrix();
}


void FrameGaborFilter::resizeEvent(QResizeEvent *)
{
    updateMatrix();
}

void FrameGaborFilter::on_splitter_splitterMoved(int pos, int index)
{
    updateMatrix();
}

void FrameGaborFilter::on_lambdaValue_valueChanged(int arg1)
{
    lambda = arg1;
    updateMatrix();
}
