#include "ImageFileManager.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


int ImageFileManager::getCurrentImageNumber()
{
    return currentImage;
}

QString ImageFileManager::getCurrentImageName()
{
    return fileList[currentImage];
}

ImageFileManager::ImageFileManager(QStringList fileList)
{
    this->fileList = fileList;
    currentImage = 0;
}

int ImageFileManager::getNumImages()
{
    return fileList.size();
}

bool ImageFileManager::isGray(const cv::Mat_<cv::Vec3b> &image)
{
    for (int y = 0; y < image.rows; y++)
        for (int x = 0; x < image.cols; x++)
            if ((image(y,x)[0] != image(y,x)[1]) ||(image(y,x)[0] != image(y,x)[2]))
                return false;
    return true;
}

cv::Mat ImageFileManager::getImage()
{
    cv::Mat image;
    QString fileName = fileList[currentImage];
    if (fileName.endsWith(".mat"))
    {
        cv::FileStorage fs(fileName.toStdString(), cv::FileStorage::READ);
        fs["mat"] >> image;
    }
    else
    {
        std::string strFilename = fileName.toStdString();
        image = cv::imread(strFilename,1);
        if (isGray(image))
            cv::cvtColor(image, image, CV_BGR2GRAY );
    }

    return image;
}


void ImageFileManager::goToImage(const int numImage)
{
    if (numImage < fileList.size())
        currentImage = numImage;
}

void ImageFileManager::nextImage()
{
    if (currentImage < fileList.size()-1)
        currentImage++;
}

void ImageFileManager::prevImage()
{
    if (currentImage > 0)
        currentImage--;
}
