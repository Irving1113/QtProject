#-------------------------------------------------
#
# Project created by QtCreator 2017-09-21T11:31:57
#
#-------------------------------------------------

QT       += core gui svg multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TSStopWatch
TEMPLATE = app

include(../TSCommon/TSCommon.pri)

SOURCES += \
    main.cpp \
    qbcirque.cpp \
    qbstopwatch.cpp

HEADERS += \
    qbcirque.h \
    qbstopwatch.h

RESOURCES += \
    images/TSStopWatch.qrc

