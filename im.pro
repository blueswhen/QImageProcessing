#-------------------------------------------------
#
# Project created by QtCreator 2014-03-27T01:56:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = imageprocessing
TEMPLATE = app


SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/utils.cpp \
    src/inpainting.cpp

HEADERS  += include/mainwindow.h \
    include/inpainting.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_imgproc.so
