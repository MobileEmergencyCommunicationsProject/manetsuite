#-------------------------------------------------
#
# Project created by QtCreator 2011-09-05T13:34:13
#
#-------------------------------------------------

QT       -= core gui
CONFIG += create_prl link_prl staticlib
INCLUDEPATH += ./include
TARGET = norm
TEMPLATE = lib

DEFINES += NORM_LIBRARY

INCLUDEPATH += $$PWD/../protolib/include
DEPENDPATH += $$PWD/../protolib

# TODO: remove normApp.cpp.
# TODO: Make targets for normApp, raft,
# normFileSend, normFileRecv, normDataSend, normDataRecv
# normStreamSend, normStreamRecv, normTest, normThreadTest
# normThreadTest2, pcap2norm, n2m, npc, ntun, fect, gtf
#

SOURCES += \
    src/common/galois.cpp \
    src/common/normSession.cpp \
    src/common/normSegment.cpp \
    src/common/normObject.cpp \
    src/common/normNode.cpp \
    src/common/normMessage.cpp \
    src/common/normFile.cpp \
    src/common/normEncoderRS16.cpp \
    src/common/normEncoderRS8.cpp \
    src/common/normEncoderMDP.cpp \
    src/common/normEncoder.cpp \
    src/common/normApi.cpp

OTHER_FILES += \
    src/common/rules.txt

HEADERS += \
    include/normVersion.h \
    include/normSimAgent.h \
    include/normSession.h \
    include/normSegment.h \
    include/normPostProcess.h \
    include/normObject.h \
    include/normNode.h \
    include/normMessage.h \
    include/normFile.h \
    include/normEncoderRS16.h \
    include/normEncoderRS8.h \
    include/normEncoderMDP.h \
    include/normEncoder.h \
    include/normApi.h \
    include/galois.h

unix:!symbian {
    DEFINES += _FILE_OFFSET_BITS=64 ECN_SUPPORT HAVE_ASSERT HAVE_DIRFD \
	HAVE_GETLOGIN HAVE_IPV6 HAVE_LOCKF \
	HAVE_OLD_SIGNALHANDLER LINUX PROTO_DEBUG UNIX

#
# NormSession needs linuxCap for Explicit Congestion Notification (ECN)
#
    SOURCES += \
    ../protolib/src/linux/linuxCap.cpp

    maemo5 {
	target.path = /opt/manetsuite/lib
    } else:contains(MEEGO_EDITION,harmattan) {
	target.path = /opt/manetsuite/lib
    } else {
	target.path = /usr/lib
    }

#    INSTALLS += target
#
# Don't install this static library
# because it makes the package too big
# to fit in /tmp on N9 phones.  /tmp
# is where QtCreator uploads package files
#
    INSTALLS -= target
}

INCLUDEPATH += $$PWD/../protolib/include
DEPENDPATH += $$PWD/../protolib

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/release/protolib.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/debug/protolib.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../protolib/libprotolib.a

