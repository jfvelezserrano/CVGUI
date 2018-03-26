#include <OperatorBuilder.h>
#include <OperatorConfigurator.h>

#include <opencv2/core/core.hpp>

#include "FrameMorphologicalFilter.h"

#ifndef MORPHOLOGYFILTERBUILDER_H
#define MORPHOLOGYFILTERBUILDER_H

class GaborFilterBuilder;


class MorphologyFilterBuilder : public OperatorConfigurator
{
    FrameMorphologicalFilter *frame;

public:
    MorphologyFilterBuilder();

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

private:
    cv::Mat getElement();
};

#endif // MORPHOLOGYFILTERBUILDER_H
