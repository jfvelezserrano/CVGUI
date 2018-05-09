#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <opencv2/core/core.hpp>

#include <QString>

class FileManager
{
public:
    virtual int getNumImages() = 0;
    virtual cv::Mat getImage()=0;
    virtual void goToImage(const int numImage) = 0;
    virtual void nextImage() = 0;
    virtual void prevImage() = 0;
    virtual int getCurrentImageNumber() = 0;
    virtual QString getCurrentImageName() = 0;
};

#endif // FILEMANAGER_H
