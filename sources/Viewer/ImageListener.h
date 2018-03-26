#ifndef IMAGELISTENER_H
#define IMAGELISTENER_H

#include <opencv2/core/core.hpp>

/**
 * @brief The ImageListener class allows to a component to be informed about changes in the ImageListener
 * over which it must actuate.
 */
class ImageListener {

public:
    /**
         * @brief Tells to an image component that the image has changed.
         * So, the component can update the allowed controls for this new image.
         * @param image The new image.
         */
    virtual void updatePanelToImage(const cv::Mat &image) = 0;
};


#endif // IMAGELISTENER_H
