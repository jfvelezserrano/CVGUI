#include "FrameCannyFilter.h"
#include "ui_FrameCannyFilter.h"

FrameCannyFilter::FrameCannyFilter(OperatorConfigurator *builder, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameCannyFilter)
{
    ui->setupUi(this);
    this->builder = builder;
}

FrameCannyFilter::~FrameCannyFilter()
{
    delete ui;
}

int FrameCannyFilter::getT1()
{
    return ui->t1SB->value();
}

int FrameCannyFilter::getT2()
{
    return ui->t2SB->value();
}

int FrameCannyFilter::getAperture()
{
    const int v = ui->apertureCB->currentText().toInt();
    return v;
}

bool FrameCannyFilter::getGradient()
{
    //return ui->L2GradientFlag->checkState() == Qt::CheckState::Checked;
}

void FrameCannyFilter::on_t1SB_valueChanged(int arg1)
{
    builder->updatePreview();
}

void FrameCannyFilter::on_t2SB_valueChanged(int arg1)
{
    builder->updatePreview();
}

void FrameCannyFilter::on_apertureSB_valueChanged(int arg1)
{
    builder->updatePreview();
}

void FrameCannyFilter::on_L2GradientFlag_stateChanged(int arg1)
{
    builder->updatePreview();
}

void FrameCannyFilter::on_apertureCB_currentIndexChanged(int index)
{
    builder->updatePreview();
}

void FrameCannyFilter::on_apertureCB_currentTextChanged(const QString &arg1)
{
    builder->updatePreview();
}
