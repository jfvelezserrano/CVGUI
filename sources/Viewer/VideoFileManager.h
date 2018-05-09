#ifndef VIDEOFILEMANAGER_H
#define VIDEOFILEMANAGER_H

#include "FileManager.h"
#include <QStringList>
#include "opencv2/highgui/highgui.hpp"

class VideoFileManager : public FileManager
{
    cv::VideoCapture video;
    cv::Mat image;
    int count;
    QString videoFileName;
public:
    VideoFileManager(QStringList fileList);
    int getNumImages();
    void goToImage(const int numImage);
    void nextImage();
    void prevImage();
    cv::Mat getImage();
    int getCurrentImageNumber();
    virtual QString getCurrentImageName();
};

#endif // VIDEOFILEMANAGER_H
