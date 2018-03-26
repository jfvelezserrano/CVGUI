#include "CannyFilterBuilder.h"


#include <opencv2/imgproc/imgproc.hpp>

CannyFilterBuilder::CannyFilterBuilder()
{
    frame = NULL;
}


std::string CannyFilterBuilder::helpURL()
{
    return "http://docs.opencv.org/modules/imgproc/doc/feature_detection.html?highlight=canny#cv2.Canny";
}

std::vector<std::string> CannyFilterBuilder::description()
{
    std::vector<std::string> description;
    description.push_back("Filter");
    description.push_back("Canny filter");
    return description;
}

bool CannyFilterBuilder::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();
    return  (image.type() == CV_8U) || (image.type() == CV_8UC3);
}


void CannyFilterBuilder::apply()
{
    cv::Mat image = getNavigator()->getImage();

    if (isCompatible())
    {
        cv::Mat output;
        cv::Canny(image,output,frame->getT1(),frame->getT2(),frame->getAperture(),frame->getGradient());
        getNavigator()->addChildImage(output,"Canny filter");
    }
}

std::string CannyFilterBuilder::getCode()
{
    std::string output = "#include <opencv2/imgproc/imgproc.hpp>";
    output += "cv::Canny(image,output,100,100);";
    return output;
}


QFrame *CannyFilterBuilder::getConfigPanel()
{
    if (frame == NULL)
    {
        frame = new FrameCannyFilter(this);
        return frame;
    }
}


cv::Mat CannyFilterBuilder::getPreviewImage()
{
    cv::Mat image = getNavigator()->getImage();

    cv::Mat output;
    if (isCompatible())
    {
        cv::Canny(image,output,frame->getT1(),frame->getT2(),frame->getAperture(),frame->getGradient());
    }
    return output;
}
