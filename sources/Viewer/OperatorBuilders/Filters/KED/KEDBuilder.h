#ifndef KEDBuilder_H
#define KEDBuilder_H

#include "KEDFrame.h"

#include <OperatorConfigurator.h>

class KEDBuilder : public OperatorConfigurator
{
    KEDFrame *frame;
public:
    std::string helpURL();
    std::vector<std::string> description();
    bool isCompatible();
    void apply();
    std::string getCode();
    virtual QFrame *getConfigPanel();
    KEDBuilder();
    cv::Mat getPreviewImage();
};

#endif // KEDBuilder_H
