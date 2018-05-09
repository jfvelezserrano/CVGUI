#ifndef IMAGEFILEMANAGER_H
#define IMAGEFILEMANAGER_H

#include "FileManager.h"
#include <QStringList>

class ImageFileManager : public FileManager
{
    QStringList fileList;
    int currentImage;
public:
    ImageFileManager(QStringList fileList);
    virtual int getNumImages();
    virtual cv::Mat getImage();
    void goToImage(const int numImage);
    void nextImage();
    void prevImage();
    int getCurrentImageNumber();
    QString getCurrentImageName();
private:
    bool isGray(const cv::Mat_<cv::Vec3b> &image);
};

#endif // IMAGEFILEMANAGER_H
