#include "RGBValuesFilterBuilder.h"

RGBValuesFilterBuilder::RGBValuesFilterBuilder()
{

}

std::string RGBValuesFilterBuilder::helpURL()
{

}

std::vector<std::string> RGBValuesFilterBuilder::description()
{
    std::vector<std::string> description;
    description.push_back("Conversions");
    description.push_back("RGB values");
    return description;
}

bool RGBValuesFilterBuilder::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();
    if (image.type() == CV_8UC3)
        return true;
    else
        return false;
}

void RGBValuesFilterBuilder::apply()
{
    cv::Mat image = getNavigator()->getImage();

    if (isCompatible())
    {
        std::vector<cv::Mat_<uchar> > rgb;
        cv::split(image, rgb);
        getNavigator()->addChildImage(rgb[2],"R values",false);
        getNavigator()->addChildImage(rgb[1],"G values",false);
        getNavigator()->addChildImage(rgb[0],"B values",true);
    }
}

std::string RGBValuesFilterBuilder::getCode()
{

}
