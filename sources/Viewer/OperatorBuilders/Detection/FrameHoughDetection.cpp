#include "FrameHoughDetection.h"
#include "ui_FrameHoughDetection.h"

#include <opencv2/ximgproc.hpp>


FrameHoughDetection::FrameHoughDetection(OperatorConfigurator *builder, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameHoughDetection)
{
    ui->setupUi(this);
    this->builder = builder;
}

FrameHoughDetection::~FrameHoughDetection()
{
    delete ui;
}

int FrameHoughDetection::getDistance()
{
    return ui->distanceResolution->value();
}

double FrameHoughDetection::getAngle()
{
    return ui->angleResolution->value();
}

int FrameHoughDetection::getThreshold()
{
    return ui->threshold->value();
}

double FrameHoughDetection::getSRN()
{
    return ui->distanceDivisor->value();
}

double FrameHoughDetection::getSTN()
{
    return ui->angleDivisor->value();
}

double FrameHoughDetection::getMinTheta()
{
    return ui->minAngle->value();
}

double FrameHoughDetection::getMaxTheta()
{
    return ui->maxAngle->value();
}

void FrameHoughDetection::on_distanceResolution_valueChanged(int arg1)
{
    builder->updatePreview();
}

void FrameHoughDetection::on_angleResolution_valueChanged(double arg1)
{
    builder->updatePreview();
}

void FrameHoughDetection::on_threshold_valueChanged(int arg1)
{
    builder->updatePreview();
}

void FrameHoughDetection::on_distanceDivisor_valueChanged(double arg1)
{
    if ((arg1 > 0) && (ui->angleDivisor->value()<0.000001))
        ui->angleDivisor->setValue(1);
    else if ((arg1<0.000001) && (ui->angleDivisor->value()>0))
        ui->angleDivisor->setValue(0);
    builder->updatePreview();
}

void FrameHoughDetection::on_angleDivisor_valueChanged(double arg1)
{
    if ((arg1 > 0) && (ui->distanceDivisor->value()<0.000000001))
        ui->distanceDivisor->setValue(1);
    else if ((arg1<0.000001) && (ui->distanceDivisor->value()>0))
        ui->distanceDivisor->setValue(0);
    builder->updatePreview();
}

void FrameHoughDetection::on_minAngle_valueChanged(const QString &arg1)
{
    builder->updatePreview();
}

void FrameHoughDetection::on_maxAngle_valueChanged(double arg1)
{
    builder->updatePreview();
}
