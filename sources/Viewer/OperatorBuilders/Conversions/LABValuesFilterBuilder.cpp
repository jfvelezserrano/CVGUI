#include "LABValuesFilterBuilder.h"

LABValuesFilterBuilder::LABValuesFilterBuilder()
{

}

std::string LABValuesFilterBuilder::helpURL()
{

}

std::vector<std::string> LABValuesFilterBuilder::description()
{
    std::vector<std::string> description;
    description.push_back("Conversions");
    description.push_back("LAB values");
    return description;
}

bool LABValuesFilterBuilder::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();
    if (image.type() == CV_8UC3)
        return true;
    else
        return false;
}

void LABValuesFilterBuilder::apply()
{
    cv::Mat image = getNavigator()->getImage();

    if (isCompatible())
    {
        cv::Mat labImage;
        cv::cvtColor(image, labImage, CV_BGR2Lab);
        std::vector<cv::Mat_<uchar> > lab;
        cv::split(labImage, lab);
        getNavigator()->addChildImage(lab[0],"L values",false);
        getNavigator()->addChildImage(lab[1],"A values",false);
        getNavigator()->addChildImage(lab[2],"B values",true);
    }
}

std::string LABValuesFilterBuilder::getCode()
{

}
