#include "HoughDetectionBuilder.h"

#include <opencv2/imgproc.hpp>
#include <vector>
#include <math.h>

HoughDetectionBuilder::HoughDetectionBuilder()
{
    frame = nullptr;
}

std::string HoughDetectionBuilder::helpURL()
{
    return "https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html";
}

std::vector<std::string> HoughDetectionBuilder::description()
{
    std::vector<std::string> description;
    description.push_back("Detection");
    description.push_back("Hough detection");
    return description;
}

bool HoughDetectionBuilder::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();
    return  (image.type() == CV_8U);
}

std::vector<std::vector<cv::Point> > HoughDetectionBuilder::getHough(cv::Mat &image)
{
    std::vector < std::vector<cv::Point> > output;

    std::vector<cv::Vec2f> lines;
    cv::HoughLines(image, lines, frame->getDistance(), frame->getAngle(), frame->getThreshold(), frame->getSRN(), frame->getSTN(), frame->getMinTheta(), frame->getMaxTheta());

   for (auto it = lines.begin(); it != lines.end(); it++)
   {
       std::vector <cv::Point> segment;
       float ro = (*it)[0];
       float theta = (*it)[1];
       cv::Point pt1(0,0);
       cv::Point pt2(static_cast<int>(ro*cos(theta)),static_cast<int>(ro*sin(theta)));

       segment.push_back(pt1);
       segment.push_back(pt2);
       output.push_back(segment);
   }

   return output;
}

void HoughDetectionBuilder::apply()
{
    cv::Mat image = getNavigator()->getImage();

    if (isCompatible())
    {
        cv::Mat obscured_image;
        cv::divide(image,5,obscured_image);
        getNavigator()->addChildImage(obscured_image,"Hough detection");
        std::vector<std::vector<cv::Point> > polygonList = getHough(image);
        for (auto it = polygonList.begin(); it != polygonList.end(); it++)
        {
            getNavigator()->addPolygon(*it);
        }
    }
}

std::string HoughDetectionBuilder::getCode()
{
    return "cv::HoughLines(image, lines, 1, CV_PI/180, 150, 0, 0);";
}

QFrame *HoughDetectionBuilder::getConfigPanel()
{
    if (frame == nullptr)
    {
        frame = new FrameHoughDetection(this);
    }
    return frame;
}

cv::Mat HoughDetectionBuilder::getPreviewImage()
{
    cv::Mat image = getNavigator()->getImage();

    cv::Mat output;
    if (isCompatible())
    {
        std::vector<std::vector<cv::Point> > polygonList = getHough(image);
    }
    return output;
}
