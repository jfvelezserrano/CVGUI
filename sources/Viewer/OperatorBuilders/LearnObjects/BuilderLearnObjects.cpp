#include "BuilderLearnObjects.h"

#include <Classification/ObjectLearner.h>


void BuilderLearnObjects::apply()
{
    cv::Mat_<cv::Vec3b> image = (cv::Mat_<cv::Vec3b>) getNavigator()->getImage();

    ObjectLearner ol;
    std::vector<std::vector<cv::Point> > polygons = ol.process(image);

    getNavigator()->addChildImage(image,"Found objects");
    getNavigator()->addPolygon(polygons[0]);
}

std::string BuilderLearnObjects::getCode()
{
    return "new LearnObjects()...";
}

QFrame* BuilderLearnObjects::getConfigPanel()
{
    if (frame == NULL)
    {
        frame = new FrameLearnObjects(this->getNavigator());
    }
    return frame;
}

void BuilderLearnObjects::updatePanelToImage(const cv::Mat &image)
{

}

std::string BuilderLearnObjects::helpURL()
{

}

std::vector<std::string> BuilderLearnObjects::description()
{
    std::vector<std::string> description;
    description.push_back("Classification");
    description.push_back("Learn objects");
    return description;
}

bool BuilderLearnObjects::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();

    if (image.type() == CV_8UC3)
        return true;
    else
        return false;
}


cv::Mat BuilderLearnObjects::getPreviewImage()
{
}
