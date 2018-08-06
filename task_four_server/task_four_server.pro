#-------------------------------------------------
#
# Project created by QtCreator 2018-07-13T10:28:37
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = task_four_server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpsocket.cpp \
    socketthread.cpp \
    server.cpp

HEADERS  += mainwindow.h \
    tcpsocket.h \
    socketthread.h \
    server.h

FORMS    += mainwindow.ui
