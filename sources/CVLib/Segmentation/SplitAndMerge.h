#ifndef SPLITANDMERGEH
#define SPLITANDMERGEH

#include <opencv2/core/core.hpp>

#include <set>



struct SplitAndMergeZoneInfo
{
    void setValues(cv::Scalar &mean, cv::Scalar &desv)
    {
        this->mean[0] = uchar(mean.val[0]);
        this->mean[1] = uchar(mean.val[1]);
        this->mean[2] = uchar(mean.val[2]);

        this->desv[0] = uchar(desv.val[0]);
        this->desv[1] = uchar(desv.val[1]);
        this->desv[2] = uchar(desv.val[2]);
    }

    cv::Vec3b mean;
    cv::Vec3b desv;
};

class SplitAndMergeDecisor
{
public:
    virtual int mustSplit(const cv::Mat_<cv::Vec3b> image, const cv::Rect_<int> &Rect_angle)=0;
    virtual bool mustMerge(const cv::Mat_<cv::Vec3b> image, const int label1, const int label2, const int x1, const int y1, const int x2, const int y2)=0;
};


/**
 * @brief The SplitAndMerge class Segment a color image using the split and merge algorithm.
 */
class SplitAndMerge
{
private:
    cv::Mat_<int> result;
    cv::Mat_<cv::Vec3b> image;
    std::vector < int > corresp;
    cv::Mat_<int> splitImage;
    cv::Mat_<int> mergeImage;

    cv::Mat_<cv::Vec3b> colorBlobImage;
    std::vector < int > zoneArea;

    SplitAndMergeDecisor *decisor;

    int numBlobs;
    int maxLabel;

    void merge();
    void split(const cv::Rect_<int> &Rect_angle);
    void filterCorresp();
    void correctLabels();
    void setBordersToZero();
    void blobAnalysis();

public:
    /**
     * @brief process Divide a image in homogeneus regions using color information.
     */
    cv::Mat_<int> process(const cv::Mat_<cv::Vec3b> &image);

    cv::Mat_<int> getSplitImage();
    cv::Mat_<int> getMergeImage();
    //cv::Mat_<short> getZoneHigh();
    //cv::Mat_<cv::Vec3b> getZoneColor();

    int getZoneInfo(const int index);
    int getNumBlobs();

    /**
     * @brief SplitAndMerge create the segmentator
    * @param StdDsvThreshold  Regions with color standar deviation beyond this threshold are splited.
    * @param ColorThreshold Regions with mean color below this threshold are joined.
     */
    SplitAndMerge(SplitAndMergeDecisor *decisor=NULL);
};


class SplitAndMergeColorDecisor : public SplitAndMergeDecisor
{
    const int COLORTHRESHOLD;
    const int STDDSVTHRESHOLD;
    cv::Mat_<cv::Vec3b> image;
    SplitAndMergeZoneInfo currentSplitInfo;
    std::vector <cv::Vec3b> zoneColor;
    cv::Mat_<uchar> highStdDev;

public:
    SplitAndMergeColorDecisor(const int StdDsvThreshold=10, const int ColorThreshold=10);
    virtual int mustSplit(const cv::Mat_<cv::Vec3b> image, const cv::Rect_<int> &region);
    virtual bool mustMerge(const cv::Mat_<cv::Vec3b> image, const int label1, const int label2, const int x1, const int y1, const int x2, const int y2);
};


class SplitAndMergeBordeDecisor : public SplitAndMergeDecisor
{
    cv::Mat_<double> integralImage;
    std::vector<double> densidades;

public:
    virtual int mustSplit(const cv::Mat_<cv::Vec3b> image, const cv::Rect_<int> &region);
    virtual bool mustMerge(const cv::Mat_<cv::Vec3b> image, const int label1, const int label2, const int x1, const int y1, const int x2, const int y2);
};


#endif // SPLITANDMERGEH
