#include "KEDBuilder.h"

#include <iostream>
#include <opencv2/highgui/highgui.hpp>

class KED
{
    const int HALF_SIZE;
    void getHistogram(const int x, const int y, const int SIZE, int *h, const cv::Mat_<uchar> &image);
    float kolmogorov(int *h1, int *h2, const float samples);
    cv::Mat_<float> ktest;
    std::vector<int*> histograms;

public:
    KED(const int HALF_SIZE);
    cv::Mat_<float> getKTest();
    cv::Mat_<int> process(const cv::Mat_<uchar> &image);
private:
    float chiSquare(int *h1, int *h2);
    void applyTransitive(std::vector<int> &adjacent);
};

KED::KED(const int HALF_SIZE):HALF_SIZE(HALF_SIZE)
{

}

/*
void KED::getHistogram(const int x, const int y, const int SIZE, int *h, const cv::Mat_<uchar> &image)
{
    for (int c= 0; c < 8; c++)
    {
        h[c]=0;
    }

    for (int f = y-1; f < y+2; f++)
    for (int c = x; c < x+SIZE; c++)
    {
        const int val = image(f,c);
        h[val/32]++;
    }

}
*/

void KED::getHistogram(const int x, const int y, const int SIZE, int *h, const cv::Mat_<uchar> &image)
{
    for (int c= 0; c < 8; c++)
        h[c]=0;

    for (int f = y; f < y+SIZE; f++)
    for (int c = x; c < x+SIZE; c++)
    {
        const int val = image(f,c);
        h[val/32]++;
    }

}

float KED::kolmogorov(int *h1, int *h2,const float samples)
{
    int maxDif = abs(h1[0]-h2[0]);
    for (int c = 1; c < 8; c++)
    {
        const int dif = abs(h1[c]-h2[c]);
        if (dif > maxDif)
            maxDif = dif;
    }
    return (double)maxDif/samples;
}


float KED::chiSquare(int *R, int *S)
{
    float output=0;

    /*
    float sumR=0,sumS=0;

    float R[8];
    float S[8];
    for (int i = 0; i < 8; i++)
    {
        R[i]=h1[i]/96.0;
        S[i]=h2[i]/96.0;

        sumR += R[i];
        sumS += S[i];
    }

    const float k1 = sqrt(sumR/sumS);
    const float k2 = sqrt(sumS/sumR);
*/
    for (int i = 0; i < 8; i++)
    {
        const float bin = R[i]-S[i];
        const float binDen = R[i]+S[i];

        if (binDen != 0)
            output += (bin*bin)/binDen;
    }
    return output;
}


cv::Mat_<float> KED::getKTest()
{
    return ktest;
}

void KED::applyTransitive(std::vector<int> &adjacent)
{
    for (int a = 0; a < adjacent.size(); a++)
    {
        int min = adjacent[a];
        while (adjacent[min] != min)
            min = adjacent[min];
        adjacent[a] = min;
    }
}


cv::Mat_<int> KED::process(const cv::Mat_<uchar> &image)
{
    const int SIZE = HALF_SIZE*2+1;
    const float THRESHOLD = 1.36/SIZE;
    const int SAMPLES = SIZE*SIZE;

    //const float THRESHOLD = 10;

    const int WIDTH = image.cols;
    const int HEIGHT = image.rows;

    std::vector<int> adjacent;
    adjacent.push_back(0);

    cv::Mat_<int> output(HEIGHT,WIDTH);
    output.setTo(0);

    int label = 0;
    int *c = new int[8];
    getHistogram(0,0,SIZE,c, image);
    histograms.push_back(c);
    c = new int[8];
    label++;


    for (int x = HALF_SIZE+1; x < WIDTH-HALF_SIZE-1; x++)
    {
        const int lcolor = output(0,x-1);
        int *l = histograms[lcolor];
        getHistogram(x-HALF_SIZE,0,SIZE,c, image);
        const float cl = kolmogorov(c,l,SAMPLES);
        //const float cl = chiSquare(c,l);
        if (cl < THRESHOLD)
        {
            output(0,x) = lcolor;
        }
        else
        {
            adjacent.push_back(label);
            output(0,x) = label;
            histograms.push_back(c);
            label++;
            c = new int[8];
        }
    }

    for (int y = HALF_SIZE; y < HEIGHT-HALF_SIZE-1; y++)
    {
        const int ucolor = output(y-1,0);
        int *u = histograms[ucolor];
        getHistogram(0,y-HALF_SIZE,SIZE,c, image);
        const float cu = kolmogorov(c,u,SAMPLES);
        //const float cu = chiSquare(c,u);
        if (cu < THRESHOLD)
        {
            output(y,0) = ucolor;
        }
        else
        {
            adjacent.push_back(label);
            output(y,0) = label;
            label++;
            histograms.push_back(c);
            c = new int[8];
        }

        for (int x = HALF_SIZE; x < WIDTH-HALF_SIZE; x++)
        {
            if ((x==3291)&&(y==84))
                int a = 2;

            const int lcolor = output(y,x-1);
            const int ucolor = output(y-1,x);

            int *u = histograms[ucolor];
            int *l = histograms[lcolor];

            getHistogram(x-HALF_SIZE,y-HALF_SIZE,SIZE,c, image);

            //const float cl = chiSquare(c,l);
            //const float cu = chiSquare(c,u);
            const float cl = kolmogorov(c,l,SAMPLES);
            const float cu = kolmogorov(c,u,SAMPLES);
            const float ul = kolmogorov(l,u,SAMPLES);

            if ((cl < THRESHOLD) && (cu < THRESHOLD))
            {
                if (lcolor == ucolor)
                    output(y,x) = ucolor;
                else if (ul < THRESHOLD)
                {
                    const int min = std::min(lcolor, ucolor);
                    const int max = std::max(lcolor, ucolor);

                    if (max == 5951)
                        int x = 2;

                    adjacent[max] = min;
                    output(y,x) = min;
                }
                else
                {
                    if (cl < cu)
                        output(y,x) = lcolor;
                    else
                        output(y,x) = ucolor;
                }
            }
            else if (cl < THRESHOLD)
            {
                output(y,x) = lcolor;
            }
            else if (cu < THRESHOLD)
            {
                output(y,x) = ucolor;
            }
            else
            {
                adjacent.push_back(label);
                output(y,x) = label;
                label++;
                histograms.push_back(c);
                c = new int[8];
            }
        }
    }

    applyTransitive(adjacent);

    for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
        output(y,x) = adjacent[output(y,x)];

    return output;
}

/*
cv::Mat_<int> KED::process(const cv::Mat_<uchar> &image)
{
    const int SIZE = 8;
    const float THRESHOLD = 30;
    const int WIDTH = image.cols/SIZE;
    const int HEIGHT = image.rows/SIZE;

    std::vector<int> adjacent;
    adjacent.push_back(0);

    cv::Mat_<int> output(HEIGHT,WIDTH);
    output.setTo(0);

    int u[8];
    int l[8];
    int c[8];

    int label = 1;

    for (int x = 1; x < WIDTH-1; x++)
    {
        getHistogram((x-1)*SIZE,0,SIZE,l, image);
        getHistogram(x*SIZE,0,SIZE,c, image);
        const float cl = chiSquare(c,l);
        if (cl < THRESHOLD)
        {
            const int lcolor = output(0,x-1);
            output(0,x) = lcolor;
        }
        else
        {
            adjacent.push_back(label);
            output(0,x) = label;
            label++;
        }
    }

    for (int y = 1; y < HEIGHT-1; y++)
    {
        getHistogram(0,y*SIZE,SIZE,c, image);
        getHistogram(0,(y-1)*SIZE,SIZE,u, image);
        const float cu = chiSquare(c,u);
        if (cu < THRESHOLD)
        {
            const int ucolor = output(y-1,0);
            output(y,0) = ucolor;
        }
        else
        {
            adjacent.push_back(label);
            output(y,0) = label;
            label++;
        }

        for (int x = 1; x < WIDTH-1; x++)
        {
//            if ((x == 128) && (y == 256))
//                int x = 2;

            getHistogram((x-1)*SIZE,y*SIZE,SIZE,l, image);
            getHistogram(x*SIZE,y*SIZE,SIZE,c, image);
            getHistogram(x*SIZE,(y-1)*SIZE,SIZE,u, image);

            const float cl = chiSquare(c,l);
            const float cu = chiSquare(c,u);

            const int lcolor = output(y,x-1);
            const int ucolor = output(y-1,x);

            if ((cl < THRESHOLD) && (cu < THRESHOLD))
            {
                if (lcolor == ucolor)
                    output(y,x) = ucolor;
                else
                {
                    const int min = std::min(lcolor, ucolor);
                    const int max = std::max(lcolor, ucolor);
                    adjacent[max] = min;
                    output(y,x) = min;
                }
            }
            else if (cl < THRESHOLD)
            {
                output(y,x) = lcolor;
            }
            else if (cu < THRESHOLD)
            {
                output(y,x) = ucolor;
            }
            else
            {
                adjacent.push_back(label);
                output(y,x) = label;
                label++;
            }
        }
    }

    applyTransitive(adjacent);

    for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
        output(y,x) = adjacent[output(y,x)];

    return output;
}
*/

/*
cv::Mat_<uchar> KED::process(const cv::Mat_<uchar> &image)
{
    const int SIZE = 32;
    const double n = SIZE*3;

    int h1[8];
    int h2[8];
    ktest.create(image.size());
    cv::Mat_<uchar> output(image.size());
    output.setTo(0);

    for (int y = 1; y < image.rows-1; y++)
        for (int x = SIZE; x < image.cols-SIZE; x++)
        {
            if ((x==369) && (y==34))
            {
               std::cout << cv::Mat(image,cv::Rect(x-SIZE,y-1,SIZE,3)) <<std::endl;
               std::cout << cv::Mat(image,cv::Rect(x+1,y-1,SIZE,3)) <<std::endl;
            }

            getHistogram(x-SIZE,y,SIZE,h1, image);
            getHistogram(x+1,y,SIZE,h2,image);
            const float s = chiSquare(h1,h2);
            ktest(y,x)=s;
        }

    for (int y = 1; y < image.rows-1; y++)
    {
        bool growing = false;
        for (int x = SIZE; x < image.cols-SIZE; x++)
        {
            if (ktest(y,x) <= ktest(y,x+1))
                growing = true;
            else if ((ktest(y,x) > ktest(y,x+1)) && growing)
            {
                if (ktest(y,x) > 100)
                    output(y,x) = 255;
                growing = false;
            }
        }
    }

    return output;
}
*/



/*
cv::Mat_<uchar> KED::process(const cv::Mat_<uchar> &image)
{
    const int SIZE = 32;
    const double n = SIZE*3;
    const float Dnn = 1.36 * sqrt((n+n)/(n*n));

    int h1[8];
    int h2[8];
    ktest.create(image.size());
    cv::Mat_<uchar> output(image.size());
    output.setTo(0);

    for (int y = 1; y < image.rows-1; y++)
        for (int x = SIZE; x < image.cols-SIZE; x++)
        {
            getHistogram(x-SIZE,y,SIZE,h1, image);
            getHistogram(x+1,y,SIZE,h2,image);
            const float s = kolmogorov(h1,h2);
            if (s > Dnn)
                ktest(y,x)=s;
            else
                ktest(y,x)=0;
        }

    for (int y = 1; y < image.rows-1; y++)
    {
        bool growing = false;
        for (int x = SIZE; x < image.cols-SIZE; x++)
        {
            if (ktest(y,x) <= ktest(y,x+1))
                growing = true;
            else if ((ktest(y,x) > ktest(y,x+1)) && growing)
            {
                output(y,x) = 255;
                growing = false;
            }
        }
    }

    return output;
}
*/


KEDBuilder::KEDBuilder()
{
    frame = NULL;
}

cv::Mat KEDBuilder::getPreviewImage()
{

}

std::string KEDBuilder::helpURL()
{

}

std::vector<std::string> KEDBuilder::description()
{
    std::vector<std::string> description;
    description.push_back("Filter");
    description.push_back("KED");
    return description;
}

bool KEDBuilder::isCompatible()
{
    cv::Mat image = getNavigator()->getImage();
    return  (image.type() == CV_8U);
}

void KEDBuilder::apply()
{
    cv::Mat image = getNavigator()->getImage();

    if (isCompatible())
    {
        KED k(frame->getWindowSize());
        cv::Mat output = k.process(image);
        getNavigator()->addChildImage(output,"KED filter");
        //getNavigator()->addChildImage(k.getKTest(),"KED energy",false);
    }
}

std::string KEDBuilder::getCode()
{

}


QFrame *KEDBuilder::getConfigPanel()
{
    if (frame == NULL)
    {
        frame = new KEDFrame();
        return frame;
    }
}

