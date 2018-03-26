
#ifndef OPERATORBUILDER_H
#define OPERATORBUILDER_H

#include "OperatorDescriptionWraper.h"

class OperatorBuilder : public OperatorDescriptionWraper
{    
    Navigator *navigator;
public:
    /**
     * Apply the filter using the navigator as source of images.
     * @return The filter.
     */
    virtual void apply() = 0;

    void setNavigator(Navigator *navigator);

    Navigator *getNavigator();

    /**
     * @return A String with the code of the filter applied.
     */
    virtual std::string getCode() = 0;
};

#endif // OPERATORBUILDER_H
