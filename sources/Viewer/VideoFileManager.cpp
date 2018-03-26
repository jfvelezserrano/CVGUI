#include "VideoFileManager.h"


VideoFileManager::VideoFileManager(QStringList fileList)
{
    video.open(fileList[0].toStdString());
    video >> image;
    count = 0;
}


int VideoFileManager::getNumImages()
{
    return video.get(CV_CAP_PROP_FRAME_COUNT);
}

cv::Mat VideoFileManager::getImage()
{
    return image;
}


void VideoFileManager::goToImage(const int numImage)
{
    video.set(CV_CAP_PROP_POS_FRAMES,numImage);
    video >> image;
    count = numImage;
}

void VideoFileManager::nextImage()
{
    video >> image;
    count++;
}

void VideoFileManager::prevImage()
{
    count--;
    video.set(CV_CAP_PROP_POS_FRAMES,count);
    video >> image;
}



int VideoFileManager::getCurrentImageNumber()
{
    int c = video.get(CV_CAP_PROP_POS_FRAMES);
    return count;
}
