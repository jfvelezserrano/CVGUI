#ifndef OBJECTLEARNER_H
#define OBJECTLEARNER_H

#include <opencv2/core/core.hpp>
#include <vector>

class ObjectLearner
{
    cv::Mat_<double> descriptors;

public:
    /**
     * @brief learnObjects Learn objects in an image.
     * @param image that contains the objects.
     * @param polygonVector Polygons corresponding to the objects to be learnt .
     */
    void learnObjects(cv::Mat_<cv::Vec3b> image, const std::vector<std::vector <cv::Point> > &polygonVector);

    /**
     * @brief process Find objects, previously learned, in an image.
     * @param image in which search the objects.
     * @return The polygons corresponding to the objects.
     */
    std::vector <std::vector <cv::Point> > process(cv::Mat_<cv::Vec3b> image);
};

#endif // OBJECTLEARNER_H
