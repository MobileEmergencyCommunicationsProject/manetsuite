#-------------------------------------------------
#
# Project created by QtCreator 2012-09-14T12:20:50
#
#-------------------------------------------------

QT       += core

#QT       -= gui

TARGET = qnormfiletransporttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

unix:!macx: LIBS += $$OUT_PWD/../qnormtransport/libqnormtransport.a $$OUT_PWD/../norm/libnorm.a $$OUT_PWD/../protolib/libprotolib.a

HEADERS += \
    testlogic.h

SOURCES += main.cpp \
    testlogic.cpp

unix:!symbian {
    DEFINES += _FILE_OFFSET_BITS=64 ECN_SUPPORT HAVE_ASSERT HAVE_DIRFD \
	HAVE_GETLOGIN HAVE_IPV6 HAVE_LOCKF \
	HAVE_OLD_SIGNALHANDLER LINUX PROTO_DEBUG UNIX

    maemo5 {
	target.path = /opt/manetsuite/bin
    } else:contains(MEEGO_EDITION,harmattan) {
	target.path = /opt/manetsuite/bin
    } else {
	target.path = /usr/bin
    }

    INSTALLS += target
}

INCLUDEPATH += $$PWD/../qnormtransport
DEPENDPATH += $$PWD/../qnormtransport

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qnormtransport/release/qnormtransport.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qnormtransport/debug/qnormtransport.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../qnormtransport/libqnormtransport.a

INCLUDEPATH += $$PWD/../norm/include
DEPENDPATH += $$PWD/../norm

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../norm/release/norm.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../norm/debug/norm.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../norm/libnorm.a

INCLUDEPATH += $$PWD/../protolib/include
DEPENDPATH += $$PWD/../protolib

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/release/protolib.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/debug/protolib.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../protolib/libprotolib.a

OTHER_FILES += \
    README.md
