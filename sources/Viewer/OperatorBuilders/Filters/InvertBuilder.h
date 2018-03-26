#ifndef INVERTBUILDER_H
#define INVERTBUILDER_H

#include <OperatorBuilder.h>



class InvertBuilder : public OperatorBuilder
{
public:
    std::string helpURL();
    std::vector<std::string> description();
    bool isCompatible();
    void apply();
    std::string getCode();
};
#endif // INVERTBUILDER_H
