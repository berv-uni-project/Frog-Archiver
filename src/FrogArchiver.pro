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
    about.cpp

HEADERS  += mainwindow.h \
    about.h \
    huffman.h \
    prioqueue.h \
    huffmantree.h

FORMS    += mainwindow.ui \
    about.ui



RESOURCES += FrogArchiver.qrc

DISTFILES +=
