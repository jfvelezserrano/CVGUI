#ifndef OPERATORDESCRIPTIONWRAPER_H
#define OPERATORDESCRIPTIONWRAPER_H

#include "FileManager.h"
#include "ImageViewer.h"
#include "Navigator.h"

#include <string>
#include <vector>

class OperatorDescriptionWraper {
public:
    virtual std::string helpURL() = 0;

    virtual std::vector <std::string> description() = 0;

    virtual bool isCompatible() = 0;
};

#endif // OPERATORDESCRIPTIONWRAPER_H
