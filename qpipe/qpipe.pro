#-------------------------------------------------
#
# Project created by QtCreator 2012-10-08T13:27:12
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = qpipe
TEMPLATE = lib
CONFIG += staticlib

SOURCES += qpipe.cpp

HEADERS += qpipe.h
unix:!symbian {
#    maemo5 {
#        target.path = /opt/usr/lib
#    } else {
#        target.path = /usr/lib
#    }
#    INSTALLS += target

    INSTALLS -= target
}
