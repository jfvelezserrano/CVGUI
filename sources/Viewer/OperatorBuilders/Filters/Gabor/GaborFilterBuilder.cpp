#include "GaborFilterBuilder.h"

#include <opencv2/imgproc/imgproc.hpp>

GaborFilterBuilder::GaborFilterBuilder()
{
    frame = NULL;
}


std::string GaborFilterBuilder::helpURL()
{
    return "http://http://en.wikipedia.org/wiki/Gabor_filter";
}

std::vector<std::string> GaborFilterBuilder::description()
{
    std::vector<std::string> description;
    description.push_back("Filter");
    description.push_back("Gabor filter");
    return description;
}

bool GaborFilterBuilder::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();
    return  (image.type() == CV_8U);
}

cv::Mat_<double> GaborFilterBuilder::getKernel()
{
    double kernelRadious = frame->getKernelRadious();
    cv::Size_<double> ksize(kernelRadious*2+1,kernelRadious*2+1);
    double sigma = frame->getSigma();
    double theta = frame->getTheta();
    double lambd = frame->getLambda();
    double gamma = frame->getGamma();
    double psi = frame->getPsi();
    cv::Mat_<double> kernel = cv::getGaborKernel(ksize, sigma, theta, lambd, gamma, psi);
    return kernel;
}

void GaborFilterBuilder::apply()
{
    cv::Mat image = getNavigator()->getImage();

    if (isCompatible())
    {
        cv::Mat_<double> kernel = getKernel();
        cv::Mat_<float> output;
        cv::filter2D(image,output,CV_32F,kernel);
        getNavigator()->addChildImage(output,"Gabor filter");
    }
}

std::string GaborFilterBuilder::getCode()
{
    std::string output = "#include <opencv2/imgproc/imgproc.hpp>";
    output += "cv::filter2D(image,output,CV_32F,kernel);";
    return output;
}


QFrame *GaborFilterBuilder::getConfigPanel()
{
    if (frame == NULL)
    {
        frame = new FrameGaborFilter(this);
        return frame;
    }
}


cv::Mat GaborFilterBuilder::getPreviewImage()
{
    cv::Mat image = getNavigator()->getImage();

    cv::Mat_<float> output;
    if (isCompatible())
    {
        cv::Mat_<double> kernel = getKernel();
        cv::filter2D(image,output,CV_32F,kernel);
    }
    return output;
}
