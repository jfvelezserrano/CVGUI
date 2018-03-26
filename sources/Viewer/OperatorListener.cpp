#include "OperatorListener.h"

OperatorListener::OperatorListener(OperatorBuilder *builder, ViewerWindow *manager) :
    QObject(NULL)
{
    this->builder = builder;
    this->viewer = manager;
}

void OperatorListener::action()
{
    OperatorConfigurator *conf = dynamic_cast <OperatorConfigurator*> (builder);
    if (conf  != NULL)
    {
        ConfigurationDialog *dialog = viewer->getConfigurator();
        dialog->addOperation(conf);
        dialog->show();
    }
    else
    {
        viewer->applyOperator(builder);
    }
    viewer->updateButtons();
}
