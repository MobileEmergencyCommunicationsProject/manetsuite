#-------------------------------------------------
#
# Project created by QtCreator 2011-09-09T18:07:47
#
#-------------------------------------------------

QT       -= core gui

CONFIG += link_prl
INCLUDEPATH += ./include
TARGET = mgen
TEMPLATE = app

#
# Meego/Harmattan comes with libnfnetlink but not the development files.
# Get them from http://harmattan-dev.nokia.com/pool/harmattan/free/libn/libnfnetlink/libnfnetlink-dev_1.0.0-2+0m6_armel.deb
# Then you can build libnetfilter-queue. Use the 20110801 shapshot.
#
unix:!macx: LIBS += -lnetfilter_queue $$OUT_PWD/../protolib/libprotolib.a

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog

unix:!symbian {
    DEFINES += _FILE_OFFSET_BITS=64 \
    _RAPR_JOURNAL \
    HAVE_ASSERT \
    HAVE_DIRFD \
    HAVE_GETLOGIN \
    HAVE_GPS \
    HAVE_IPV6 \
    HAVE_LOCKF \
    HAVE_PSELECT \
    HAVE_SCHED \
    LINUX \
    PROTO_DEBUG \
    UNIX

    maemo5 {
	target.path = /opt/manetsuite/bin
    } else:contains(MEEGO_EDITION,harmattan) {
	target.path = /opt/manetsuite/bin
    } else {
	target.path = /usr/bin
    }

    scripts.path = /opt/manetsuite/bin
    scripts.files = ../scripts/*.sh

    INSTALLS += scripts
    INSTALLS += target
}

SOURCES += \
    src/common/gpsPub.cpp \
    src/common/mgen.cpp \
    src/common/mgenApp.cpp \
    src/common/mgenAppSinkTransport.cpp \
    src/common/mgenEvent.cpp \
    src/common/mgenFlow.cpp \
    src/common/mgenMsg.cpp \
    src/common/mgenPattern.cpp \
    src/common/mgenPayload.cpp \
    src/common/mgenSequencer.cpp \
    src/common/mgenTransport.cpp

HEADERS += \
    include/mgenVersion.h \
    include/mgenTransport.h \
    include/mgenSequencer.h \
    include/mgenPayload.h \
    include/mgenPattern.h \
    include/mgenMsg.h \
    include/mgenGlobals.h \
    include/mgenFlow.h \
    include/mgenEvent.h \
    include/mgenApp.h \
    include/mgen.h \
    include/gpsPub.h

INCLUDEPATH += $$PWD/../protolib/include
DEPENDPATH += $$PWD/../protolib

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/release/protolib.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/debug/protolib.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../protolib/libprotolib.a
