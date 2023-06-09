#-------------------------------------------------
#
# Project created by QtCreator 2021-03-26T19:48:32
#
#-------------------------------------------------

QT       += core gui network multimedia multimediawidgets texttospeech sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = park
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += resources_big
SOURCES += \
    WaterDrop.cpp \
    http.cpp \
    image.cpp \
        main.cpp \
        mainwidget.cpp \
    mainwindow.cpp \
    registwindow.cpp \
    screenwidget.cpp \
    subwidget.cpp \
    pay.cpp

HEADERS += \
    WaterDrop.h \
    http.h \
    image.h \
        mainwidget.h \
    mainwindow.h \
    registwindow.h \
    screenwidget.h \
    subwidget.h \
    pay.h

FORMS += \
        registwindow.ui \
        widget.ui

RC_FILE = myicon.rc

RESOURCES += \
    pic.qrc \
    qss.qrc \
    res.qrc
