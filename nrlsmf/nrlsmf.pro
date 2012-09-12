#-------------------------------------------------
#
# Project created by QtCreator 2011-09-09T18:07:47
#
#-------------------------------------------------

QT       -= core gui
CONFIG += link_prl
TARGET = nrlsmf
TEMPLATE = app


#
# Don't know if protolib for OS X is a library or a framework.
# Does it matter?
#
#
# Meego/Harmattan comes with libnfnetlink but not the development files.
# Get them from http://harmattan-dev.nokia.com/pool/harmattan/free/libn/libnfnetlink/libnfnetlink-dev_1.0.0-2+0m6_armel.deb
# Then you can build libnetfilter-queue. Use the 20110801 shapshot.
#
unix:!macx: LIBS += -lnetfilter_queue -lnfnetlink $$OUT_PWD/../protolib/libprotolib.a
#-lpcap

SOURCES += \
    common/smfHashSHA1.cpp \
    common/smfHashMD5.cpp \
    common/smfHash.cpp \
    common/smfDpd.cpp \
    common/smf.cpp \
    common/nrlsmf.cpp

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog

HEADERS += \
    common/smfVersion.h \
    common/smfHashSHA1.h \
    common/smfHashMD5.h \
    common/smfHash.h \
    common/smfDpd.h \
    common/smf.h

unix:!symbian {
    DEFINES += _FILE_OFFSET_BITS=64 \
    HAVE_ASSERT HAVE_DIRFD \
    HAVE_GETLOGIN HAVE_IPV6 \
    HAVE_LOCKF HAVE_OLD_SIGNALHANDLER \
    HAVE_SCHED \
    LINUX \
    MAEMO \
    MNE_SUPPORT NO_SCM_RIGHTS PROTO_DEBUG SMF_SUPPORT UNIX

    INCLUDEPATH += ../protolib/include/unix

    SOURCES += ../protolib/src/linux/linuxCap.cpp \
    ../protolib/src/linux/linuxDetour.cpp

#
# Defining aligned_be64 is a hack to work around the broken or missing
# mechanism for determining for linuxDetour.cpp (see SOURCES, above) which
# flavor of prototype libnetfilter_queue.h defines for
#
# nfq_get_payload()
#
# For Maemo5, we want the one chosen when align_be64 is defined.
#
   maemo5 {
	DEFINES += aligned_be64
	target.path = /opt/manetsuite/bin
    } else:contains(MEEGO_EDITION,harmattan) {
	DEFINES += aligned_be64
	target.path = /opt/manetsuite/bin
    } else {
	target.path = /usr/bin
    }

    INSTALLS += target
}

INCLUDEPATH += $$PWD/../protolib/include
DEPENDPATH += $$PWD/../protolib

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/release/protolib.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/debug/protolib.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../protolib/libprotolib.a
