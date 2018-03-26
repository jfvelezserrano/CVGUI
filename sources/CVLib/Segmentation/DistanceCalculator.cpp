#include "DistanceCalculator.h"

#include <list>

void DistanceCalculator::explorePoint(const int x, const int y, const float N, cv::Mat_<float> &output, std::list <int> &auxXStack, std::list <int> &auxYStack)
{
   if ((x >=0) && (y >=0) && (x < output.cols) && (y < output.rows) && (output(y,x) > N))
   {
     output(y,x) = N;
     auxXStack.push_back(x);
     auxYStack.push_back(y);
   }
 }

cv::Mat_<float> DistanceCalculator::process(const cv::Mat_<uchar> &image)
{
    const int WIDTH = image.cols;
    const int HEIGHT = image.rows;

    cv::Mat_<float> output(image.size());
    output.setTo(WIDTH*HEIGHT);

    int blackPoints = 0;

    std::list <int> xStack;
    std::list <int> yStack;

    //Copy black pixels
    for (int y = 0; y < HEIGHT; y++)
    for (int x = 0; x < WIDTH; x++)
    {
      if (image(y,x) == 0)
      {
        xStack.push_back(x);
        yStack.push_back(y);
        output(y,x) = 0;
        blackPoints++;
      }
    }

    unsigned int N = 1;

    //While the stack is not empty
      //Get one element from the stack
      //Insert in the stack the rounding pixels whose value > N, put N to these pixels and store in stack2
    //N = N+1

    while (!xStack.empty())
    {
      std::list <int> auxXStack;
      std::list <int> auxYStack;

      const int TAMANO_ACTUAL = xStack.size();

      std::list <int>::iterator it_x = xStack.end();
      std::list <int>::iterator it_y = yStack.end();

      for (int cont = 0; cont < TAMANO_ACTUAL; cont++)
      {
        it_x--;
        it_y--;
        int x = *it_x;
        int y = *it_y;

//        explorePoint(x-1,y-1,N,output,auxXStack,auxYStack);
        explorePoint(x,  y-1,N,output,auxXStack,auxYStack);
//        explorePoint(x+1,y-1,N,output,auxXStack,auxYStack);

        explorePoint(x-1,y,N,output,auxXStack,auxYStack);
        explorePoint(x+1,y,N,output,auxXStack,auxYStack);

//        explorePoint(x-1,y+1,N,output,auxXStack,auxYStack);
        explorePoint(x,  y+1,N,output,auxXStack,auxYStack);
//        explorePoint(x+1,y+1,N,output,auxXStack,auxYStack);
      }
      N = N + 1;

      xStack = auxXStack;
      yStack = auxYStack;
    }

    return output;
}
