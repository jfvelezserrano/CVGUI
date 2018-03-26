#include "ScaleColorAsigner.h"
#include <opencv2/imgproc/imgproc.hpp>


cv::Mat_<cv::Vec3b> ScaleColorAsigner::assing(cv::Mat_<float> image)
{
    cv::Mat_<uchar> hue;
    cv::normalize(image,hue,0,150,cv::NORM_MINMAX);

    cv::Mat_<uchar> max(image.size());
    max.setTo(255);

    std::vector<cv::Mat> channels;
    channels.push_back(hue);
    channels.push_back(max);
    channels.push_back(max);
    int from_to[] = { 0,0, 1,1, 2,2 };

    std::vector<cv::Mat> outputMix;
    cv::Mat_<cv::Vec3b> output(image.size());
    outputMix.push_back(output);
    cv::mixChannels(channels,outputMix,from_to,3);
    cv::cvtColor(output,output,CV_HSV2BGR);

    return output;
}

cv::Mat_<cv::Vec3b> ScaleColorAsigner::assing(cv::Mat_<double> image)
{
    cv::Mat_<uchar> hue;
    cv::normalize(image,hue,0,179,cv::NORM_MINMAX);
    hue = 179-hue;

    cv::Mat_<uchar> max(image.size());
    max.setTo(255);

    std::vector<cv::Mat> channels;
    channels.push_back(hue);
    channels.push_back(max);
    channels.push_back(max);
    int from_to[] = { 0,0, 1,1, 2,2 };

    std::vector<cv::Mat> outputMix;
    cv::Mat_<cv::Vec3b> output(image.size());
    outputMix.push_back(output);
    cv::mixChannels(channels,outputMix,from_to,3);
    cv::cvtColor(output,output,CV_HSV2BGR);

    return output;
}
