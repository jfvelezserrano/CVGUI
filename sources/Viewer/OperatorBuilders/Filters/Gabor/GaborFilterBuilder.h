#include <OperatorBuilder.h>
#include <OperatorConfigurator.h>

#include "FrameGaborFilter.h"

#include <opencv2/core/core.hpp>

#ifndef GABORFILTERBUILDER_H
#define GABORFILTERBUILDER_H

class GaborFilterBuilder  : public OperatorConfigurator
{
    FrameGaborFilter *frame;

public:
    GaborFilterBuilder();
    cv::Mat_<double> getKernel();

    // OperatorDescriptionWraper interface
public:
    std::string helpURL();
    std::vector<std::string> description();
    bool isCompatible();

    // OperatorBuilder interface
public:
    void apply();
    std::string getCode();

    // OperatorConfigurator interface
public:
    QFrame *getConfigPanel();
    cv::Mat getPreviewImage();
};

#endif // GABORFILTERBUILDER_H
