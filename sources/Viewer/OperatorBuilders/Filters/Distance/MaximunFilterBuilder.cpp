#include "MaximunFilterBuilder.h"

std::string MaximunFilterBuilder::helpURL()
{

}

std::vector<std::string> MaximunFilterBuilder::description()
{
    std::vector<std::string> description;
    description.push_back("Filter");
    description.push_back("Maximun");
    return description;
}

bool MaximunFilterBuilder::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();
    return  (image.type() == CV_8U) || (image.type() == CV_32F) || (image.type() == CV_32S)|| (image.type() == CV_64F);
}

template<class T> cv::Mat_<uchar> MaximunFilterBuilder::applyType(cv::Mat_<T> image)
{
    cv::Mat_<uchar> output(image.size());
    output.setTo(0);

    for (int y = 1; y < image.rows-1; y++)
        for (int x = 1; x < image.cols-1; x++)
        {
            const T v = image(y,x);
            if ((v > image(y,x+1)) &&
                    (v > image(y,x-1)) &&
                    (v > image(y-1,x+1)) &&
                    (v > image(y-1,x)) &&
                    (v > image(y-1,x-1)) &&
                    (v > image(y+1,x+1)) &&
                    (v > image(y+1,x)) &&
                    (v > image(y+1,x-1)))
                output(y,x) = 255;
        }

    return output;
}

void MaximunFilterBuilder::apply()
{
    cv::Mat image = getNavigator()->getImage();

    cv::Mat_<uchar> output;
    if (image.type() == CV_8U)
        output = applyType<uchar>(image);
    else if (image.type() == CV_32S)
        output = applyType<int>(image);
    if (image.type() == CV_32F)
        output = applyType<float>(image);
    if (image.type() == CV_64F)
        output = applyType<double>(image);

    getNavigator()->addChildImage(output,"Maximuns");
}

std::string MaximunFilterBuilder::getCode()
{

}

