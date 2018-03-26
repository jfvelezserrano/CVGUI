#-------------------------------------------------
#
# Project created by QtCreator 2014-07-07T07:49:40
#
#-------------------------------------------------

QT       -= gui

TARGET = CVLib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += Segmentation/SplitAndMerge.cpp \
    Classification/ObjectLearner.cpp \
    Segmentation/DistanceCalculator.cpp

HEADERS += Segmentation/SplitAndMerge.h \
    Classification/ObjectLearner.h \
    Segmentation/DistanceCalculator.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
