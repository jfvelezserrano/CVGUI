#ifndef BRIGHTVALUESFILTERBUILDER_H
#define BRIGHTVALUESFILTERBUILDER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <OperatorBuilder.h>

class BrightValuesFilterBuilder : public OperatorBuilder
{
public:
    BrightValuesFilterBuilder();

    // OperatorDescriptionWraper interface
public:
    std::string helpURL();
    std::vector<std::string> description();
    bool isCompatible();

    // OperatorBuilder interface
public:
    void apply();
    std::string getCode();
};

#endif // BRIGHTVALUESFILTERBUILDER_H
