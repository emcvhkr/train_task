#-------------------------------------------------
#
# Project created by QtCreator 2018-06-15T16:38:28
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog_settings.cpp \
    util.cpp

HEADERS  += mainwindow.h \
    dialog_settings.h \
    util.h

FORMS    += mainwindow.ui \
    dialog_settings.ui

RESOURCES += \
    res.qrc

TRANSLATIONS += 360safe_zh.ts 360safe_en.ts
