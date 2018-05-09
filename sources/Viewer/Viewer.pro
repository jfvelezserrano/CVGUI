#-------------------------------------------------
#
# Project created by QtCreator 2014-04-11T10:21:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Viewer
TEMPLATE = app

CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += opencv

SOURCES += main.cpp\
        ViewerWindow.cpp \
    ScaleColorAsigner.cpp \
    RandomColorAsigner.cpp \
    Navigator.cpp \
    ImageViewer.cpp \
    OperatorBuilderLoader.cpp \
    OperatorListener.cpp \
    ConfigurationDialog.cpp \
    OperatorBuilders/LearnObjects/BuilderLearnObjects.cpp \
    OperatorBuilders/LearnObjects/FrameLearnObjects.cpp \
    OperatorBuilders/SplitAndMerge/SplitAndMergeBuilder.cpp \
    ImageViewerPolygonSelection.cpp \
    ImageFileManager.cpp \
    VideoFileManager.cpp \
    OperatorBuilders/Conversions/BrightValuesFilterBuilder.cpp \
    Histograms.cpp \
    ImageViewerPointSelection.cpp \
    ImageViewerRectangleSelection.cpp \
    ImageViewerWithSelector.cpp \
    OperatorBuilder.cpp \
    OperatorConfigurator.cpp \
    LogWindow.cpp \
    OperatorBuilders/Conversions/RGBValuesFilterBuilder.cpp \
    OperatorBuilders/Conversions/HSVValuesFilterBuilder.cpp \
    OperatorBuilders/Conversions/LABValuesFilterBuilder.cpp \
    OperatorBuilders/SplitAndMerge/SplitAndMergeFrame.cpp \
    OperatorBuilders/Filters/Canny/CannyFilterBuilder.cpp \
    OperatorBuilders/Filters/Gabor/FrameGaborFilter.cpp \
    OperatorBuilders/Filters/Gabor/GaborFilterBuilder.cpp \
    OperatorBuilders/Filters/Canny/FrameCannyFilter.cpp \
    OperatorBuilders/Filters/InvertBuilder.cpp \
    OperatorBuilders/Filters/Distance/DistanceFilterBuilder.cpp \
    OperatorBuilders/Filters/Distance/MaximunFilterBuilder.cpp \
    OperatorBuilders/Filters/KED/KEDBuilder.cpp \
    OperatorBuilders/Filters/KED/KEDFrame.cpp \
    OperatorBuilders/Filters/Morphology/MorphologicalFilterBuilder.cpp \
    OperatorBuilders/Filters/Morphology/FrameMorphologicalFilter.cpp \
    OperatorBuilders/Detection/FrameHoughDetection.cpp \
    OperatorBuilders/Detection/HoughDetectionBuilder.cpp

HEADERS  += ViewerWindow.h \
    ScaleColorAsigner.h \
    RandomColorAsigner.h \
    Navigator.h \
    OperatorBuilder.h \
    ImageViewer.h \
    FileManager.h \
    OperatorConfigurator.h \
    ImageListener.h \
    OperatorDescriptionWraper.h \
    OperatorBuilderLoader.h \
    OperatorListener.h \
    ConfigurationDialog.h \
    OperatorBuilders/LearnObjects/BuilderLearnObjects.h \
    OperatorBuilders/LearnObjects/FrameLearnObjects.h \
    OperatorBuilders/SplitAndMerge/SplitAndMergeBuilder.h \
    ImageViewerPolygonSelection.h \
    ImageFileManager.h \
    VideoFileManager.h \
    OperatorBuilders/Conversions/BrightValuesFilterBuilder.h \
    Histograms.h \
    ImageViewerPointSelection.h \
    ImageViewerRectangleSelection.h \
    ImageViewerSelection.h \
    ImageViewerWithSelector.h \
    LogWindow.h \
    OperatorBuilders/Conversions/RGBValuesFilterBuilder.h \
    OperatorBuilders/Conversions/HSVValuesFilterBuilder.h \
    OperatorBuilders/Conversions/LABValuesFilterBuilder.h \
    OperatorBuilders/SplitAndMerge/SplitAndMergeFrame.h \
    OperatorBuilders/Filters/Canny/FrameCannyFilter.h \
    OperatorBuilders/Filters/Canny/CannyFilterBuilder.h \
    OperatorBuilders/Filters/Gabor/FrameGaborFilter.h \
    OperatorBuilders/Filters/Gabor/GaborFilterBuilder.h \
    OperatorBuilders/Filters/InvertBuilder.h \
    OperatorBuilders/Filters/Distance/DistanceFilterBuilder.h \
    OperatorBuilders/Filters/Distance/MaximunFilterBuilder.h \
    OperatorBuilders/Filters/KED/KEDBuilder.h \
    OperatorBuilders/Filters/KED/KEDFrame.h \
    OperatorBuilders/Filters/Morphology/FrameMorphologicalFilter.h \
    OperatorBuilders/Filters/Morphology/MorphologicalFilterBuilder.h \
    OperatorBuilders/Detection/FrameHoughDetection.h \
    OperatorBuilders/Detection/HoughDetectionBuilder.h

FORMS    += ViewerWindow.ui \
    Navigator.ui \
    ConfigurationDialog.ui \
    OperatorBuilders/LearnObjects/FrameLearnObjects.ui \
    Histograms.ui \
    LogWindow.ui \
    OperatorBuilders/SplitAndMerge/SplitAndMergeFrame.ui \
    OperatorBuilders/Filters/Gabor/FrameGaborFilter.ui \
    OperatorBuilders/Filters/Canny/FrameCannyFilter.ui \
    OperatorBuilders/Filters/KED/KEDFrame.ui \
    OperatorBuilders/Filters/Morphology/FrameMorphologicalFilter.ui \
    OperatorBuilders/Detection/FrameHoughDetection.ui

RESOURCES += \
    Viewer.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CVLib/release/ -lCVLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CVLib/debug/ -lCVLib
else:unix: LIBS += -L$$OUT_PWD/../CVLib/ -lCVLib

INCLUDEPATH += $$PWD/../CVLib
DEPENDPATH += $$PWD/../CVLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CVLib/release/libCVLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CVLib/debug/libCVLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CVLib/release/CVLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../CVLib/debug/CVLib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../CVLib/libCVLib.a
