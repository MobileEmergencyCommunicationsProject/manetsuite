#-------------------------------------------------
#
# Project created by QtCreator 2012-10-09T13:58:49
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = qpipetest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += link_prl

TEMPLATE = app

INCLUDES +=
SOURCES += main.cpp \
    pipetest.cpp \
    receiver.cpp \
    sender.cpp

unix:!macx:!symbian: LIBS += -L$$OUT_PWD/../qpipe/ -lqpipe

INCLUDEPATH += $$PWD/../qpipe
DEPENDPATH += $$PWD/../qpipe

unix:!macx:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../qpipe/libqpipe.a

HEADERS += \
    pipetest.h \
    receiver.h \
    sender.h
