#ifndef MAXIMUNFILTERBUILDER_H
#define MAXIMUNFILTERBUILDER_H

#include <OperatorBuilder.h>



class MaximunFilterBuilder : public OperatorBuilder
{
public:
    std::string helpURL();
    std::vector<std::string> description();
    bool isCompatible();
    void apply();
    std::string getCode();
private:
    template<class T>
    cv::Mat_<uchar> applyType(cv::Mat_<T> image);
};


#endif // MAXIMUNFILTERBUILDER_H
