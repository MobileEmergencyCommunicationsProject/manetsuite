#-------------------------------------------------
#
# Project created by QtCreator 2012-09-28T11:56:07
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = normfileinfoiteratortest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

LIBS += $$OUT_PWD/../libqnormtransport.a \
	$$OUT_PWD/../../norm/libnorm.a \
	$$OUT_PWD/../../protolib/libprotolib.a

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../release/qnormtransport.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../debug/qnormtransport.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../libqnormtransport.a

INCLUDEPATH += $$PWD/../../norm/include
DEPENDPATH += $$PWD/../../norm

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../norm/release/norm.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../norm/debug/norm.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../../norm/libnorm.a

INCLUDEPATH += $$PWD/../../protolib/include
DEPENDPATH += $$PWD/../../protolib

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protolib/release/protolib.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../protolib/debug/protolib.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../../protolib/libprotolib.a
