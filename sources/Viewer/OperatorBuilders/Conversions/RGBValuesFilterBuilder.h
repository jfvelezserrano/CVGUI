#ifndef RGBVALUESFILTERBUILDER_H
#define RGBVALUESFILTERBUILDER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <OperatorBuilder.h>

class RGBValuesFilterBuilder : public OperatorBuilder
{
public:
    RGBValuesFilterBuilder();
public:
    std::string helpURL();
    std::vector<std::string> description();
    bool isCompatible();

    // OperatorBuilder interface
public:
    void apply();
    std::string getCode();
};

#endif // RGBVALUESFILTERBUILDER_H
