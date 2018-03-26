#ifndef BUILDERLEARNOBJECTS_H
#define BUILDERLEARNOBJECTS_H

#include "FrameLearnObjects.h"

#include "ImageListener.h"
#include "OperatorConfigurator.h"
#include "OperatorBuilder.h"

class BuilderLearnObjects : public OperatorConfigurator, public ImageListener
{
private:
    FrameLearnObjects *frame = NULL;

public:
    virtual void apply();

    virtual std::string getCode();

    virtual std::string helpURL();

    virtual std::vector<std::string> description();

    virtual bool isCompatible();

    virtual QFrame *getConfigPanel();

    virtual void updatePanelToImage(const cv::Mat &image);

    cv::Mat getPreviewImage();
};

#endif // BUILDERLEARNOBJECTS_H
