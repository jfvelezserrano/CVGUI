#ifndef SCALECOLORASIGNER_H
#define SCALECOLORASIGNER_H

#include <opencv2/core/core.hpp>

class ScaleColorAsigner
{
public:
    static cv::Mat_<cv::Vec3b> assing(cv::Mat_<float> image);
    static cv::Mat_<cv::Vec3b> assing(cv::Mat_<double> image);
};

#endif // SCALECOLORASIGNER_H
