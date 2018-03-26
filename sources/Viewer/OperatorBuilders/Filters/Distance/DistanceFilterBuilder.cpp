#include "DistanceFilterBuilder.h"

#include <Segmentation/DistanceCalculator.h>


std::string DistanceFilterBuilder::helpURL()
{

}

std::vector<std::string> DistanceFilterBuilder::description()
{
    std::vector<std::string> description;
    description.push_back("Filter");
    description.push_back("Distance");
    return description;
}

bool DistanceFilterBuilder::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();
    return  (image.type() == CV_8U);
}

void DistanceFilterBuilder::apply()
{
    cv::Mat image = getNavigator()->getImage();

    if (isCompatible())
    {
        DistanceCalculator d;
        cv::Mat_<float> output = d.process(image);
        getNavigator()->addChildImage(output,"Distance filter");
    }
}

std::string DistanceFilterBuilder::getCode()
{

}
