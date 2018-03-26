#include "OperatorBuilder.h"

void OperatorBuilder::setNavigator(Navigator *navigator)
{
    this->navigator=navigator;
}

Navigator* OperatorBuilder::getNavigator()
{
    return navigator;
}
