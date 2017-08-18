#-------------------------------------------------
#
# Project created by QtCreator 2017-08-18T15:18:15
#
#-------------------------------------------------

QT       += core gui
QT +=concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled6
TEMPLATE = app


CONFIG += C++14
QMAKE_CXXFLAGS += -std=c++14
SOURCES += main.cpp\
        mainwindow.cpp \
    tfm.cpp

HEADERS  += mainwindow.h \
    mime.h \
    mymodel.h \
    tfm.h \
    thr.h \
    treeproxy.h

FORMS    += mainwindow.ui
