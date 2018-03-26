#ifndef HSVVALUESFILTERBUILDER_H
#define HSVVALUESFILTERBUILDER_H


#include <opencv2/imgproc/imgproc.hpp>
#include <OperatorBuilder.h>


class HSVValuesFilterBuilder: public OperatorBuilder
{
public:
    HSVValuesFilterBuilder();
public:
    std::string helpURL();
    std::vector<std::string> description();
    bool isCompatible();

    // OperatorBuilder interface
public:
    void apply();
    std::string getCode();
};

#endif // HSVVALUESFILTERBUILDER_H
