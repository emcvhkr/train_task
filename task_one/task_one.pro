#-------------------------------------------------
#
# Project created by QtCreator 2018-06-14T23:48:05
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = task_one
TEMPLATE = app

TRANSLATIONS += 360safe_zh.ts 360safe_en.ts


SOURCES += main.cpp\
        mainwindow.cpp \
    ntoolbutton.cpp \
    npushbutton.cpp \
    titlewidget.cpp \
    contentwidget.cpp \
    util.cpp \
    mainmenu.cpp \
    changeskinwidget.cpp \
    skinwidget.cpp \
    aboutus.cpp \
    settingdialog.cpp \
    clabel.cpp \
    character.cpp \
    systemtray.cpp

HEADERS  += mainwindow.h \
    ntoolbutton.h \
    common.h \
    titlewidget.h \
    contentwidget.h \
    util.h \
    mainmenu.h \
    skinwidget.h \
    changeskinwidget.h \
    aboutus.h \
    npushbutton.h \
    settingdialog.h \
    clabel.h \
    character.h \
    systemtray.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
