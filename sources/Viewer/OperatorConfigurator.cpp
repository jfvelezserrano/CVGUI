#include "OperatorConfigurator.h"

void OperatorConfigurator::setPreviewViewer(ImageViewer *viewer)
{
    this->viewer = viewer;
}

void OperatorConfigurator::updatePreview()
{
    if (viewer->isVisible())
        viewer->setImage(getPreviewImage());
}
