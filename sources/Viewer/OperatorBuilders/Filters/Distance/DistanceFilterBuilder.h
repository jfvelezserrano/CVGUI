#ifndef DISTANCEFILTERBUILDER_H
#define DISTANCEFILTERBUILDER_H

#include <OperatorBuilder.h>



class DistanceFilterBuilder: public OperatorBuilder
{
public:
    std::string helpURL();
    std::vector<std::string> description();
    bool isCompatible();
    void apply();
    std::string getCode();
};

#endif // DISTANCEFILTERBUILDER_H
