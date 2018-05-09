#include "FrameHoughDetection.h"
#include "ui_FrameHoughDetection.h"

#include <opencv2/ximgproc.hpp>


FrameHoughDetection::FrameHoughDetection(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameHoughDetection)
{
    ui->setupUi(this);
}

FrameHoughDetection::~FrameHoughDetection()
{
    delete ui;
}
