#-------------------------------------------------
#
# Project created by QtCreator 2018-06-28T09:38:11
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = task_two
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    nvideowidget.cpp \
    recordthread.cpp \
    playermenu.cpp

HEADERS  += mainwindow.h \
    nvideowidget.h \
    recordthread.h \
    playermenu.h

FORMS    += mainwindow.ui
