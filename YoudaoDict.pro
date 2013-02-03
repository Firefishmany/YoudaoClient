#-------------------------------------------------
#
# Project created by QtCreator 2013-02-03T12:55:40
#
#-------------------------------------------------

QT       += core gui network

LIBS += "-lqjson"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YoudaoDict
TEMPLATE = app


SOURCES += main.cpp\
        youdaowindow.cpp

HEADERS  += youdaowindow.h

FORMS    += youdaowindow.ui

RESOURCES += \
    resources.qrc
