QT += core
QT += gui

CONFIG += c++11

TARGET = project-serv
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    catalog.cpp \
    core.cpp \
    recog.cpp

HEADERS += \
    catalog.h \
    core.h \
    recog.h

LIBS += /usr/lib/libopencv_core.so \
    /usr/lib/libopencv_features2d.so \
    /usr/lib/libopencv_highgui.so \
    /usr/lib/libopencv_contrib.so \
    /usr/lib/libopencv_flann.so \
    /usr/lib/libopencv_calib3d.so \
    /usr/lib/libopencv_imgproc.so \
    /usr/lib/libopencv_legacy.so \
    /usr/lib/libopencv_nonfree.so
