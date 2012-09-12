#-------------------------------------------------
#
# Project created by QtCreator 2011-09-05T13:34:13
#
#-------------------------------------------------

QT       -= core gui
CONFIG += link_prl
#DEFINES += NRLOLSR_LIBRARY
INCLUDEPATH += ./common
TARGET = nrlolsrd
TEMPLATE = app

#
# Don't know what libs to use for OS X and Symbian
#
unix:!macx:!symbian: LIBS += -lpthread -ldl $$OUT_PWD/../protolib/libprotolib.a

SOURCES += \
    ./common/olsr_packet_types.cpp \
    ./common/nrlolsrApp.cpp \
    ./common/nrlolsr.cpp \
    ./common/nbr_queue.cpp \
    ./common/mprselPub.cpp

HEADERS += \
    ./common/olsr_packet_types.h \
    ./common/nrlolsrgui.h \
    ./common/nrlolsrApp.h \
    ./common/nrlolsr.h \
    ./common/nbr_queue.h \
    ./common/mprselPub.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE3432B2A
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = nrlolsr.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    DEFINES += HAVE_ASSERT HAVE_DIRFD \
    HAVE_GETLOGIN HAVE_IPV6 \
    HAVE_LOCKF HAVE_OLD_SIGNALHANDLER \
    LINUX PROTO_DEBUG SMF_SUPPORT UNIX

    INCLUDEPATH += ../protolib/include/unix

    maemo5 {
	target.path = /opt/manetsuite/bin
    } else:contains(MEEGO_EDITION,harmattan) {
	target.path = /opt/manetsuite/bin
    } else {
	target.path = /usr/bin
    }

    INSTALLS += target

    SOURCES += ../protolib/src/linux/linuxCap.cpp
}

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog

INCLUDEPATH += $$PWD/../protolib/include
DEPENDPATH += $$PWD/../protolib

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/release/protolib.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/debug/protolib.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../protolib/libprotolib.a
