#ifndef VIDEOFILEMANAGER_H
#define VIDEOFILEMANAGER_H

#include "FileManager.h"

#include "opencv2/highgui/highgui.hpp"

class VideoFileManager : public FileManager
{
    cv::VideoCapture video;
    cv::Mat image;
    int count;
public:
    VideoFileManager(QStringList fileList);
    int getNumImages();
    void goToImage(const int numImage);
    void nextImage();
    void prevImage();
    cv::Mat getImage();
    int getCurrentImageNumber();
};

#endif // VIDEOFILEMANAGER_H
