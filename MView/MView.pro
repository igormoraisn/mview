#-------------------------------------------------
#
# Project created by QtCreator 2018-01-02T14:35:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MView
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    getmaterials.cpp \
    makesourcefile.cpp \
    connection.cpp

HEADERS  += mainwindow.h \
    getmaterials.h \
    makesourcefile.h

FORMS    += mainwindow.ui
