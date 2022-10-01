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
    extractwindow.cpp \
    compresswindow.cpp

HEADERS  += mainwindow.h \
    about.h \
    prioqueue.h \
    huffmantree.h \
    huffmanencoding.h \
    huffmandecoding.h \
    license.h \
    extractwindow.h \
    compresswindow.h

FORMS    += mainwindow.ui \
    about.ui \
    license.ui \
    extractwindow.ui \
    compresswindow.ui

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += FrogArchiver.qrc

TRANSLATIONS = \
    i18n/FrogArchiver_en.ts \
    i18n/FrogArchiver_es.ts \
    i18n/FrogArchiver_fr.ts \

DISTFILES += \
    config/config.xml \
    packages/org.berviantoleo.frogarchiver/packages.xml
