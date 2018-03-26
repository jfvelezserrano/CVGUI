#include "FrameLearnObjects.h"
#include "ui_FrameLearnObjects.h"

#include <QFileDialog>

FrameLearnObjects::FrameLearnObjects(Navigator *navigator, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameLearnObjects)
{
    ui->setupUi(this);
    this->navigator = navigator;
}

FrameLearnObjects::~FrameLearnObjects()
{
    delete ui;
}

void FrameLearnObjects::on_newLearner_clicked()
{
    QString filter = "Pattern database (*.pdb);;All (*.*)";
    QString selectedFilter ="Pattern Database (*.pdb)";

    lastFileName = QFileDialog::getSaveFileName(this,"New pattern database", lastFileName, filter, &selectedFilter);

    //Borramos los descriptores
}

void FrameLearnObjects::on_loadLearner_clicked()
{
    QString filter = "Pattern Database (*.pdb);;All (*.*)";
    QString selectedFilter ="Pattern Database (*.pdb)";

    QStringList fileNames = QFileDialog::getOpenFileNames(this,"Open pattern database",lastFileName,filter,&selectedFilter,QFileDialog::ReadOnly);

    if (fileNames.size() > 0)
    {
        //Cargamos los descriptores y el umnbral
    }
}

void FrameLearnObjects::on_addLayer_clicked()
{
    //Añadimos los descriptores al fichero
}
