#include "HSVValuesFilterBuilder.h"

HSVValuesFilterBuilder::HSVValuesFilterBuilder()
{

}

std::string HSVValuesFilterBuilder::helpURL()
{

}

std::vector<std::string> HSVValuesFilterBuilder::description()
{
    std::vector<std::string> description;
    description.push_back("Conversions");
    description.push_back("HSV values");
    return description;
}

bool HSVValuesFilterBuilder::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();
    if (image.type() == CV_8UC3)
        return true;
    else
        return false;
}

void HSVValuesFilterBuilder::apply()
{
    cv::Mat image = getNavigator()->getImage();

    if (isCompatible())
    {
        cv::Mat hsvImage;
        cv::cvtColor(image, hsvImage, CV_BGR2HSV);
        std::vector<cv::Mat_<uchar> > hsv;
        cv::split(hsvImage, hsv);
        getNavigator()->addChildImage(hsv[0],"H values",false);
        getNavigator()->addChildImage(hsv[1],"S values",false);
        getNavigator()->addChildImage(hsv[2],"V values",true);
    }
}

std::string HSVValuesFilterBuilder::getCode()
{

}
