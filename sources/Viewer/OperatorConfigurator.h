#include "Navigator.h"
#include "ImageViewer.h"
#include "OperatorBuilder.h"

#ifndef OPERATORCONFIGURATOR_H
#define OPERATORCONFIGURATOR_H

/**
 * @brief The OperatorConfigurator class return the configuration window for a operation.
 */
class OperatorConfigurator : public OperatorBuilder {
    ImageViewer *viewer;
public:
    /**
     * The method return the configuration window for the operation.
     * Ideally, first call to this method create the configuration window.
     * @param viewer
     * @param navigator
     * @return A configuration panel.
     */
    virtual QFrame* getConfigPanel() = 0;

    virtual cv::Mat getPreviewImage() = 0;

    void setPreviewViewer(ImageViewer *viewer);

    void updatePreview();

};

#endif // OPERATORCONFIGURATOR_H
