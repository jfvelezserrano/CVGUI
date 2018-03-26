#include "SplitAndMergeBuilder.h"


#include <Segmentation/SplitAndMerge.h>

#include <opencv2/imgproc/imgproc.hpp>


void SplitAndMergeBuilder::apply()
{
    cv::Mat image = getNavigator()->getImage();

    if (image.type() == CV_8UC3)
    {
        SplitAndMerge sp;
        cv::Mat_<int> result = sp.process((cv::Mat_<cv::Vec3b>)image);
        getNavigator()->addChildImage(result,"Split and merge");
        getNavigator()->addChildImage(sp.getSplitImage(),"Split",false);
    }
}

std::string SplitAndMergeBuilder::getCode()
{
    std::string output = "#include <Segmentation/SplitAndMerge.h>";
    output += "SplitAndMerge sp(10,10);";
    output += "cv::Mat_<int> result = sp.process((cv::Mat_<cv::Vec3b>)image);";
    return output;
}

std::string SplitAndMergeBuilder::helpURL()
{
    return "";
}

std::vector <std::string> SplitAndMergeBuilder::description()
{
    std::vector<std::string> description;
    description.push_back("Detection");
    description.push_back("Split and merge");
    return description;
}

bool SplitAndMergeBuilder::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();

    if (image.type() == CV_8UC3)
        return true;
    else
        return false;
}

cv::Mat SplitAndMergeBuilder::getPreviewImage()
{
    cv::Mat image = getNavigator()->getImage();
    cv::Mat_<int> result;

    if (image.type() == CV_8UC3)
    {
        int rows,cols;
        if (image.rows > image.cols)
        {
            rows = 1000;
            cols = image.cols * 1000 / image.rows;
        }
        else
        {
            cols = 1000;
            rows = image.rows * 1000 / image.cols;
        }

        cv::resize(image,image,cv::Size(cols,rows));
        SplitAndMerge sp;
        result = sp.process((cv::Mat_<cv::Vec3b>)image);
    }
    return result;
}


QFrame *SplitAndMergeBuilder::getConfigPanel()
{
    if (frame == NULL)
    {
        frame = new SplitAndMergeFrame();
    }
    return frame;
}

void SplitAndMergeBuilder::updatePanelToImage(const cv::Mat &image)
{

}

