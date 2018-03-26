#ifndef RANDOMCOLORASIGNER_H
#define RANDOMCOLORASIGNER_H

#include <opencv2/core/core.hpp>

#include <vector>

class RandomColorAsigner
{
    static std::vector<cv::Vec3b> palette;
public:
    static cv::Mat_<cv::Vec3b> assing(cv::Mat_<int> image);

private:
    static cv::Vec3b hsv2rgb(const float h, const float s, const float v);
    static std::vector<cv::Vec3b> initPalete();
};

#endif // RANDOMCOLORASIGNER_H
