#-------------------------------------------------
#
# Project created by QtCreator 2012-08-23T20:18:46
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = messure
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    sysbutton.cpp \
    mypushbutton.cpp \
    titlewidget.cpp \
    toolbutton.cpp \
    toolwidget.cpp \
    statuswidget.cpp \
    contentwidget.cpp \
    skinwidget.cpp \
    frminput.cpp \
    statusinfo.cpp \
    recordinfo.cpp \
    collectionthread.cpp \
    infobean.cpp \
    clickedlabel.cpp \
    uiutils.cpp \
    fileexportwidget.cpp \
    bridgeresistor.cpp \
    ad7124.c \
    Communication.c \
    meter.c \
    gpio_ctl.c

HEADERS  += mainwidget.h \
    sysbutton.h \
    mypushbutton.h \
    titlewidget.h \
    toolbutton.h \
    toolwidget.h \
    statuswidget.h \
    skinwidget.h \
    contentwidget.h\
    frminput.h \
    statusinfo.h \
    recordinfo.h \
    collectionthread.h \
    infobean.h \
    clickedlabel.h \
    uiutils.h \
    fileexportwidget.h \
    bridgeresistor.h \
    AD7124.h \
    AD7124_regs.h \
    Communication.h \
    debug.h \
    meter.h \
    gpio_ctl.h
RESOURCES += \
    src.qrc

FORMS += frminput.ui

DISTFILES += \
    img/toolWidget/export.png


