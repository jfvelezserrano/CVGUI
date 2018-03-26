#include "SplitAndMerge.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


cv::Mat_<int> SplitAndMerge::process(const cv::Mat_<cv::Vec3b> &image)
{
    this->image = image;
    cv::Rect_<int> rectangle(0,0,image.cols,image.rows);
    result = cv::Mat_<int>(image.size());
    maxLabel = 0;

    split(rectangle);
    splitImage = result.clone();

    corresp.resize(maxLabel,0);
    for (int cont = 0; cont <= maxLabel; cont++)
        corresp[cont] = cont;

    merge();
    filterCorresp();
    correctLabels();

    mergeImage = result.clone();

    //setBordersToZero();

    numBlobs = corresp.size();
    blobAnalysis();

    return result;
}

void SplitAndMerge::setBordersToZero()
{
    const cv::Vec3b black(0,0,0);

    for (int y = 0; y < image.rows; y++)
    for (int x = 0; x < image.cols;  x++)
    {
        if (image(y,x) == black)
            result(y,x) = 0;
    }
}


int SplitAndMerge::getNumBlobs()
{
    return numBlobs;
}


void SplitAndMerge::split(const cv::Rect_<int> &rectangle)
{
    const int newLabel = decisor->mustSplit(image,rectangle);

    if (newLabel > maxLabel)
        maxLabel = newLabel;

    if (newLabel == 0)
    {
        float aspectRatio = (float) rectangle.width / rectangle.height;

        if ( (aspectRatio > 0.9) && (aspectRatio < 1.1) )
        {
            const int x0 = rectangle.x;
            const int y0 = rectangle.y;
            const int w0 = std::max(1,rectangle.width/2);
            const int h0 = std::max(1,rectangle.height/2);
            const int x1 = x0 + w0;
            const int y1 = y0 + h0;
            const int w1 = std::max(1,rectangle.width - w0);
            const int h1 = std::max(1,rectangle.height - h0);

            const cv::Rect_<int> q0(x0,y0,w0,h0);
            const cv::Rect_<int> q1(x1,y0,w1,h0);
            const cv::Rect_<int> q2(x0,y1,w0,h1);
            const cv::Rect_<int> q3(x1,y1,w1,h1);

            split(q0);
            split(q1);
            split(q2);
            split(q3);
        }
        else if (rectangle.width > rectangle.height)
        {
            const int x0 = rectangle.x;
            const int y0 = rectangle.y;
            const int w0 = std::max(1,rectangle.width/2);
            const int h0 = std::max(1,rectangle.height);
            const int x1 = x0 + w0;
            const int w1 = std::max(1,rectangle.width - w0);
            const cv::Rect_<int> q0(x0,y0,w0,h0);
            const cv::Rect_<int> q1(x1,y0,w1,h0);
            split(q0);
            split(q1);
        }
        else
        {
            const int x0 = rectangle.x;
            const int y0 = rectangle.y;
            const int w0 = std::max(1,rectangle.width);
            const int h0 = std::max(1,rectangle.height/2);
            const int y1 = y0 + h0;
            const int h1 = std::max(1,rectangle.height - h0);
            const cv::Rect_<int> q0(x0,y0,w0,h0);
            const cv::Rect_<int> q1(x0,y1,w0,h1);
            split(q0);
            split(q1);
        }
    }
    else
    {
        for (int y = rectangle.y; y < rectangle.y+rectangle.height;y++)
        for (int x = rectangle.x; x < rectangle.x+rectangle.width;x++)
        {
            result(y,x) = newLabel;
        }
    }
}


void SplitAndMerge::merge()
{
    for (int y = 0; y < image.rows;y++)
    for (int x = 0; x < image.cols;x++)
    {
        const int currentLabel = result(y,x);
        const int upLabel = y>0 ? result(y-1,x):-1;
        const int leftLabel = x>0 ? result(y,x-1):-1;

        if ((currentLabel>=0) && (upLabel>=0) && (currentLabel != upLabel))
        {
            const bool mergeUp = decisor->mustMerge(image,currentLabel,upLabel,x,y,x,y-1);

            if (mergeUp)
            {
                const int minLabel = std::min(corresp[currentLabel],corresp[upLabel]);

                if (corresp[currentLabel] > minLabel)
                    corresp[corresp[currentLabel]] = minLabel;
                if (corresp[upLabel] > minLabel)
                    corresp[corresp[upLabel]] = minLabel;

                corresp[currentLabel] = corresp[minLabel];
                corresp[upLabel] = corresp[minLabel];
            }
        }
        if ((currentLabel>=0) && (leftLabel>=0) && (currentLabel != leftLabel))
        {
            const bool mergeLeft = decisor->mustMerge(image,currentLabel, leftLabel, x, y, x-1, y);

            if (mergeLeft)
            {
                const int minLabel = std::min(corresp[currentLabel],corresp[leftLabel]);

                if (corresp[currentLabel] > minLabel)
                    corresp[corresp[currentLabel]] = minLabel;
                if (corresp[leftLabel] > minLabel)
                    corresp[corresp[leftLabel]] = minLabel;

                corresp[currentLabel] = corresp[minLabel];
                corresp[leftLabel] = corresp[minLabel];
            }
        }
    }
}

void SplitAndMerge::filterCorresp()
{
    for (unsigned int cont = 0; cont < corresp.size(); cont++)
    {
        int a = cont;
        int b = corresp[a];

        while (b != corresp[b])
        {
            b = corresp[b];
        }

        corresp[a]=b;
    }
}


void SplitAndMerge::correctLabels()
{
    for(int j=0; j<result.rows; j++)
    {
        for(int i=0; i<result.cols; i++)
        {
            if (result(j,i) >= 0)
                result(j,i)=corresp[result(j,i)];
        }
    }
}


cv::Mat_<int> SplitAndMerge::getSplitImage()
{
    return splitImage;
}

cv::Mat_<int> SplitAndMerge::getMergeImage()
{
    return mergeImage;
}


void SplitAndMerge::blobAnalysis()
{
    std::vector <long long int> zoneR;
    std::vector <long long int> zoneG;
    std::vector <long long int> zoneB;

    std::vector <long long int> zoneH;
    std::vector <long long int> zoneS;
    std::vector <long long int> zoneV;

    std::vector <long long int> zoneHigh;

    std::vector <int> area;
    std::vector <int> areaHigh;

    zoneR.resize(numBlobs,0);
    zoneG.resize(numBlobs,0);
    zoneB.resize(numBlobs,0);
    zoneHigh.resize(numBlobs,0);
    area.resize(numBlobs,0);
    areaHigh.resize(numBlobs,0);

    for (int y = 0; y < image.rows;y++)
    for (int x = 0; x < image.cols;x++)
    {
        const cv::Vec3b color = image(y,x);
        const int label = result(y,x);

        zoneR[label] += color[2];
        zoneG[label] += color[1];
        zoneB[label] += color[0];

        area[label]++;
    }

    //zoneColor.clear();
    corresp.clear();
    zoneArea.clear();

    //zoneColor.resize(numBlobs,0);
    corresp.resize(numBlobs,0);
    zoneArea.resize(numBlobs,0);

    for (int count = 0; count < numBlobs; count++)
    {
        const int areaDiv = area[count];
        corresp[count] = count;
        if (areaDiv > 0)
        {
            //zoneColor[count] = cv::Vec3b(uchar(zoneB[count]/areaDiv), uchar(zoneG[count]/areaDiv), uchar(zoneR[count]/areaDiv));
            zoneArea[count] = areaDiv;
        }
    }
}

SplitAndMerge::SplitAndMerge(SplitAndMergeDecisor *decisor)
{
    if (decisor == NULL)
       //this->decisor = new SplitAndMergeBordeDecisor();
       this->decisor = new SplitAndMergeColorDecisor();
    else
        this->decisor = decisor;
}


SplitAndMergeColorDecisor::SplitAndMergeColorDecisor(const int StdDsvThreshold, const int ColorThreshold):COLORTHRESHOLD(ColorThreshold),STDDSVTHRESHOLD(StdDsvThreshold)
{
    zoneColor.push_back(cv::Vec3b(0,0,0));
}

int SplitAndMergeColorDecisor::mustSplit(const cv::Mat_<cv::Vec3b> image, const cv::Rect_<int> &region)
{
    const int STDDSVTHRESHOLD = 10;

    if (highStdDev.rows == 0)
    {
        highStdDev.create(image.size());
        highStdDev.setTo(0);
    }

    cv::Mat_<cv::Vec3b> imageZone(image,region);
    cv::Scalar mean;
    cv::Scalar stdDev;
    cv::meanStdDev(imageZone, mean, stdDev);

    currentSplitInfo.setValues(mean,stdDev);


    if ((region.width < 2) || (region.height < 2))
    {
        zoneColor.push_back(currentSplitInfo.mean);

        for (int y = region.y; y < region.y+region.height;y++)
        for (int x = region.x; x < region.x+region.width;x++)
        {
            highStdDev(y,x) = 255;
        }

        return zoneColor.size()-1;
    }

    if ((mean.val[0] == 0) && (mean.val[1] == 0) && (mean.val[2] == 0))
    {
        zoneColor.push_back(currentSplitInfo.mean);
        return zoneColor.size()-1;
    }

    if ((stdDev.val[0] < STDDSVTHRESHOLD) && (stdDev.val[1] < STDDSVTHRESHOLD) && (stdDev.val[2] < STDDSVTHRESHOLD))
    {
        zoneColor.push_back(currentSplitInfo.mean);
        return zoneColor.size()-1;
    }

    return 0;
}

bool SplitAndMergeColorDecisor::mustMerge(const cv::Mat_<cv::Vec3b> image, const int label1, const int label2, const int x1, const int y1, const int x2, const int y2)
{
    static cv::Vec3b black = cv::Vec3b(0,0,0);
    const cv::Vec3b &color1 = zoneColor[label1];
    const cv::Vec3b &color2 = zoneColor[label2];

    if ((color1 == black) || (color2 == black))
        return false;

    const int highDesv1 = highStdDev(y1,x1);
    const int highDesv2 = highStdDev(y2,x2);

    if ((highDesv1 == highDesv2) && (highDesv1 == 255))
        return true;

    const int rDistance = std::abs(color1[2] - color2[2]);
    const int gDistance = std::abs(color1[1] - color2[1]);
    const int bDistance = std::abs(color1[0] - color2[0]);

    if ((rDistance < COLORTHRESHOLD) && (gDistance < COLORTHRESHOLD) && (bDistance < COLORTHRESHOLD) && (highDesv1 == highDesv2))
        return true;

    return false;
}



int SplitAndMergeBordeDecisor::mustSplit(const cv::Mat_<cv::Vec3b> image, const cv::Rect_<int> &region)
{
    if (integralImage.rows == 0)
    {
        cv::Mat_<uchar> edgeImage;
        //cv::cvtColor(image,edgeImage,CV_BGR2GRAY);
        cv::Canny(image,edgeImage,100,100);
        cv::integral(edgeImage,integralImage,CV_64F);
        densidades.push_back(0);
    }

    const double w = integralImage(region.y,region.x);
    const double x = integralImage(region.y+region.height,region.x+region.width);
    const double y = integralImage(region.y,region.x+region.width);
    const double z = integralImage(region.y+region.height,region.x);

    const double sum = x-y-z + w;
    if (sum <= 0)
    {
        const double densidad = sum / (region.height * region.width);
        densidades.push_back(densidad);
        return densidades.size()-1;
    }
    else if ((region.width == 1) && (region.height == 1))
    {
        const double densidad = sum / (region.height * region.width);
        densidades.push_back(densidad);
        return densidades.size()-1;
    }
    else
        return 0;
}


bool SplitAndMergeBordeDecisor::mustMerge(const cv::Mat_<cv::Vec3b> image, const int label1, const int label2, const int x1, const int y1, const int x2, const int y2)
{
    const double dif = abs(densidades[label1] - densidades[label2]);

    return (dif == 0);
}
