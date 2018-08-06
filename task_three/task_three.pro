#-------------------------------------------------
#
# Project created by QtCreator 2018-07-05T09:37:38
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = task_three
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    portsendthread.cpp \
    portsender.cpp

HEADERS  += mainwindow.h \
    portsendthread.h \
    portsender.h

FORMS    += mainwindow.ui
