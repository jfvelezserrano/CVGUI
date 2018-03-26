#include "BrightValuesFilterBuilder.h"

BrightValuesFilterBuilder::BrightValuesFilterBuilder()
{

}

std::string BrightValuesFilterBuilder::helpURL()
{

}

std::vector<std::string> BrightValuesFilterBuilder::description()
{
    std::vector<std::string> description;
     description.push_back("Conversions");
     description.push_back("Bight values");
     return description;
}

bool BrightValuesFilterBuilder::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();
    if (image.type() == CV_8UC3)
        return true;
    else
        return false;
}

void BrightValuesFilterBuilder::apply()
{
    cv::Mat image = getNavigator()->getImage();

    if (isCompatible())
    {
        cv::Mat_<uchar> output;
        cv::cvtColor(image,output,CV_BGR2GRAY);
        getNavigator()->addChildImage(output,"Bright values");
    }
}

std::string BrightValuesFilterBuilder::getCode()
{

}
