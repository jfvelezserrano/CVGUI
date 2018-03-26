#ifndef CANNYFILTERBUILDER_H
#define CANNYFILTERBUILDER_H

#include <OperatorBuilder.h>
#include <OperatorConfigurator.h>
#include "FrameCannyFilter.h"


class CannyFilterBuilder: public OperatorConfigurator
{
    FrameCannyFilter *frame;

public:
    CannyFilterBuilder();


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

#endif // CANNYFILTERBUILDER_H
