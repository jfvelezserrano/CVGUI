#ifndef OPERATORBUILDERLOADER_H
#define OPERATORBUILDERLOADER_H

#include "OperatorBuilder.h"

#include <list>

class OperatorBuilderLoader
{
public:
    std::list<OperatorBuilder*> getOperators();
};

#endif // OPERATORBUILDERLOADER_H
