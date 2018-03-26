#ifndef DISTANCECALCULATOR_H
#define DISTANCECALCULATOR_H

#include <opencv2/core/core.hpp>
#include <list>

class DistanceCalculator
{
    void explorePoint(const int x, const int y, const float N, cv::Mat_<float> &bitmap_out, std::list <int> &pila_x_aux, std::list <int> &pila_y_aux);

public:
    cv::Mat_<float> process(const cv::Mat_<uchar> &image);
};

#endif // DISTANCECALCULATOR_H
