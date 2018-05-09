#ifndef HOUGHDETECTIONBUILDER_H
#define HOUGHDETECTIONBUILDER_H

#include <opencv2/core/core.hpp>
#include <OperatorBuilder.h>
#include <OperatorConfigurator.h>

class HoughDetectionBuilder : public OperatorConfigurator
{
public:
    HoughDetectionBuilder();

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

#endif // HOUGHDETECTIONBUILDER_H
