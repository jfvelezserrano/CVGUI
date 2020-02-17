#include "OperatorBuilderLoader.h"

#include <OperatorBuilders/LearnObjects/BuilderLearnObjects.h>
#include <OperatorBuilders/SplitAndMerge/SplitAndMergeBuilder.h>
#include <OperatorBuilders/Filters/Morphology/MorphologicalFilterBuilder.h>
#include <OperatorBuilders/Filters/Gabor/GaborFilterBuilder.h>
#include <OperatorBuilders/Filters/Canny/CannyFilterBuilder.h>
#include <OperatorBuilders/Conversions/BrightValuesFilterBuilder.h>
#include <OperatorBuilders/Conversions/RGBValuesFilterBuilder.h>
#include <OperatorBuilders/Conversions/HSVValuesFilterBuilder.h>
#include <OperatorBuilders/Conversions/LABValuesFilterBuilder.h>
#include <OperatorBuilders/Filters/InvertBuilder.h>
#include <OperatorBuilders/Filters/KED/KEDBuilder.h>
#include <OperatorBuilders/Filters/Distance/DistanceFilterBuilder.h>
#include <OperatorBuilders/Filters/Distance/MaximunFilterBuilder.h>
#include <OperatorBuilders/Detection/HoughDetectionBuilder.h>

std::list<OperatorBuilder *> OperatorBuilderLoader::getOperators()
{
    std::list <OperatorBuilder*> l;

    l.push_back(new BrightValuesFilterBuilder());
    l.push_back(new RGBValuesFilterBuilder());
    l.push_back(new HSVValuesFilterBuilder());
    l.push_back(new LABValuesFilterBuilder());

    l.push_back(new GaborFilterBuilder());
    l.push_back(new CannyFilterBuilder());
    l.push_back(new MorphologyFilterBuilder());
    l.push_back(new InvertBuilder());

    l.push_back(new SplitAndMergeBuilder());
    l.push_back(new BuilderLearnObjects());
    l.push_back(new DistanceFilterBuilder());
    l.push_back(new MaximunFilterBuilder());
    l.push_back(new KEDBuilder());
    l.push_back(new HoughDetectionBuilder());

    return l;
}
