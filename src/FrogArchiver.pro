#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T19:15:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FrogArchiver
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    about.cpp \
    huffmanencoding.cpp \
    huffmandecoding.cpp \
    license.cpp \
    dialogembed.cpp \
    extractwindow.cpp

HEADERS  += mainwindow.h \
    about.h \
    prioqueue.h \
    huffmantree.h \
    huffmanencoding.h \
    huffmandecoding.h \
    license.h \
    dialogembed.h \
    extractwindow.h

FORMS    += mainwindow.ui \
    about.ui \
    license.ui \
    compress-window.ui \
    extractwindow.ui

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += FrogArchiver.qrc

DISTFILES +=
