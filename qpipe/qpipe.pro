#-------------------------------------------------
#
# Project created by QtCreator 2012-10-08T13:27:12
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = qpipe
TEMPLATE = lib
#CONFIG += staticlib
CONFIG += create_prl link_prl staticlib

SOURCES += qpipe.cpp

HEADERS += qpipe.h \
    QPipe
unix:!symbian {
#    maemo5 {
#        target.path = /opt/usr/lib
#    } else {
#        target.path = /usr/lib
#    }

#    INSTALLS += target
#
# Don't install this static library
# because it makes the package too big
# to fit in /tmp on N9 phones.  /tmp
# is where QtCreator uploads package files
#
    INSTALLS -= target
}

maemo5 {
    target.path = /opt/qpipe/lib
    INSTALLS += target
}

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/qpipe/lib
    INSTALLS += target
}
