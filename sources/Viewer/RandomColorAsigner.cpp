#include "RandomColorAsigner.h"

#include <cstdlib>

std::vector<cv::Vec3b> RandomColorAsigner::palette = RandomColorAsigner::initPalete();

std::vector<cv::Vec3b> RandomColorAsigner::initPalete()
{
  std::vector<cv::Vec3b> palette;

  for (int cont = 0; cont < 256; cont++)
  {
    int randValue = rand()%360;
    cv::Vec3b color = hsv2rgb(randValue,1,1);
    palette.push_back(color);
  }

  return palette;
}

cv::Vec3b RandomColorAsigner::hsv2rgb(const float h, const float s, const float v)
{
    cv::Vec3b         out;

    if (s <= 0.0)
    {
        out[2] = v*255;
        out[1] = v*255;
        out[0] = v*255;
        return out;
    }

    double hh = h;
    if (hh >= 360.0)
        hh = 0.0;
    hh /= 60.0;

    const long i = (long)hh;

    const double ff = hh - i;
    const double p = v * (1.0 - s);
    const double q = v * (1.0 - (s * ff));
    const double t = v * (1.0 - (s * (1.0 - ff)));

    switch(i) {
    case 0:
        out[2] = v*255;
        out[1] = t*255;
        out[0] = p*255;
        break;
    case 1:
        out[2] = q*255;
        out[1] = v*255;
        out[0] = p*255;
        break;
    case 2:
        out[2] = p*255;
        out[1] = v*255;
        out[0] = t*255;
        break;

    case 3:
        out[2] = p*255;
        out[1] = q*255;
        out[0] = v*255;
        break;
    case 4:
        out[2] = t*255;
        out[1] = p*255;
        out[0] = v*255;
        break;
    case 5:
    default:
        out[2] = v*255;
        out[1] = p*255;
        out[0] = q*255;
        break;
    }
    return out;
}

cv::Mat_<cv::Vec3b> RandomColorAsigner::assing(cv::Mat_<int> image)
{
    cv::Mat_<cv::Vec3b> output(image.size());

    for (int y = 0; y < image.rows; y++)
    for (int x = 0; x < image.cols; x++)
    {
        const int label = image(y,x);
        output(y,x) = palette[label%256];
    }
    return output;
}
