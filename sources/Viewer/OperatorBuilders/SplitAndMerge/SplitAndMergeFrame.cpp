#include "SplitAndMergeFrame.h"
#include "ui_SplitAndMergeFrame.h"

SplitAndMergeFrame::SplitAndMergeFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SplitAndMergeFrame)
{
    ui->setupUi(this);
}

SplitAndMergeFrame::~SplitAndMergeFrame()
{
    delete ui;
}
