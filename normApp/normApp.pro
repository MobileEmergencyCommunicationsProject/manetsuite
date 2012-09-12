TEMPLATE = app
CONFIG += console link_prl
CONFIG -= qt
TARGET = norm

unix:!macx: LIBS += $$OUT_PWD/../norm/libnorm.a $$OUT_PWD/../protolib/libprotolib.a

SOURCES += ../norm/src/common/normPostProcess.cpp \
    ../norm/src/common/normApp.cpp \
    ../norm/src/unix/unixPostProcess.cpp

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
