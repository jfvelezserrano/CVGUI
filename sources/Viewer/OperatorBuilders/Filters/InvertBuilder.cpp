#include "InvertBuilder.h"

std::string InvertBuilder::helpURL()
{

}

std::vector<std::string> InvertBuilder::description()
{
    std::vector<std::string> description;
    description.push_back("Filter");
    description.push_back("Invert");
    return description;
}

bool InvertBuilder::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();
    return  (image.type() == CV_8U);
}

void InvertBuilder::apply()
{
    cv::Mat image = getNavigator()->getImage();

    if (isCompatible())
    {
        cv::Mat output = ~image;
        getNavigator()->addChildImage(output,"Negative filter");
    }
}

std::string InvertBuilder::getCode()
{

}
