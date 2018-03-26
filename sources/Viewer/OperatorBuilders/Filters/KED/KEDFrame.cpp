#include "KEDFrame.h"
#include "ui_KEDFrame.h"

KEDFrame::KEDFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::KEDFrame)
{
    ui->setupUi(this);
}

KEDFrame::~KEDFrame()
{
    delete ui;
}

int KEDFrame::getWindowSize()
{
    int v = ui->windowSizeSB->value();
    return v;
}
