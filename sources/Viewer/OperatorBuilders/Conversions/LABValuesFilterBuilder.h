#ifndef LABVALUESFILTERBUILDER_H
#define LABVALUESFILTERBUILDER_H


#include <opencv2/imgproc/imgproc.hpp>
#include <OperatorBuilder.h>


class LABValuesFilterBuilder : public OperatorBuilder
{
public:
    LABValuesFilterBuilder();
public:
    std::string helpURL();
    std::vector<std::string> description();
    bool isCompatible();

    // OperatorBuilder interface
public:
    void apply();
    std::string getCode();
};


#endif // LABVALUESFILTERBUILDER_H
