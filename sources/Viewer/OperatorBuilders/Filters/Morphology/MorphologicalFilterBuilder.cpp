#include "MorphologicalFilterBuilder.h"
#include "FrameMorphologicalFilter.h"

#include <opencv2/imgproc/imgproc.hpp>\

MorphologyFilterBuilder::MorphologyFilterBuilder()
{
    frame = NULL;
}

std::string MorphologyFilterBuilder::helpURL()
{
    return "https://docs.opencv.org/trunk/d9/d61/tutorial_py_morphological_ops.html";
}


std::vector<std::string> MorphologyFilterBuilder::description()
{
    std::vector<std::string> description;
    description.push_back("Filter");
    description.push_back("Morphological filter");
    return description;
}

bool MorphologyFilterBuilder::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();
    return  (image.type() == CV_8U) || (image.type() == CV_8UC3);
}

void MorphologyFilterBuilder::apply()
{
    cv::Mat image = getNavigator()->getImage();

    if (isCompatible())
    {        
        cv::Mat element = frame->getElement();
        cv::Mat output;
        if (frame->isDilate())
            cv::dilate(image,output,element);
        else
            cv::erode(image,output,element);
        getNavigator()->addChildImage(output,"Morphological filter");
    }
}



std::string MorphologyFilterBuilder::getCode()
{
    std::string output = "#include <opencv2/imgproc/imgproc.hpp>\n";
    output += "cv::getStructuringElement(MORPH_RECT,Size(3,3),Point(1,1));\n";
    output += "cv::dilation(image,output,element);\n";
    return output;
}


QFrame *MorphologyFilterBuilder::getConfigPanel()
{
    if (frame == NULL)
    {
        frame = new FrameMorphologicalFilter(this);
        return frame;
    }
}


cv::Mat MorphologyFilterBuilder::getPreviewImage()
{
    cv::Mat image = getNavigator()->getImage();

    cv::Mat output;
    if (isCompatible())
    {
        cv::Mat element = frame->getElement();

        if (frame->isDilate())
            cv::dilate(image,output,element);
        else
            cv::erode(image,output,element);
    }
    return output;
}
