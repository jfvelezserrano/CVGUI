#ifndef SPLIRANDMERGEBUILDER_H
#define SPLIRANDMERGEBUILDER_H

#include "ImageListener.h"
#include "OperatorConfigurator.h"
#include "OperatorBuilder.h"

#include "SplitAndMergeFrame.h"

class SplitAndMergeBuilder : public OperatorConfigurator, public ImageListener
{
private:
    SplitAndMergeFrame *frame;

public:
    virtual void apply();

    virtual std::string getCode();

    virtual std::string helpURL();

    virtual std::vector <std::string> description();

    virtual bool isCompatible();

    virtual QFrame *getConfigPanel();

    virtual void updatePanelToImage(const cv::Mat &image);

    cv::Mat getPreviewImage();
};
#endif // SPLIRANDMERGEBUILDER_H
