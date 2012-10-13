#-------------------------------------------------
#
# Project created by QtCreator 2011-09-02T11:05:50
#
#-------------------------------------------------

QT       -= core gui

#CONFIG += create_prl link_prl staticlib
CONFIG += create_prl link_prl staticlib
TARGET = protolib
TEMPLATE = lib

DEFINES += PROTOLIB_LIBRARY

INCLUDEPATH += ./include

#unix:!macx:!symbian: LIBS += -ldl -lrt

SOURCES += \
    ./src/common/protoVif.cpp \
    ./src/common/protoTree.cpp \
    ./src/common/protoTimer.cpp \
    ./src/common/protoTime.cpp \
    ./src/common/protoSpace.cpp \
    ./src/common/protoSocket.cpp \
    ./src/common/protoSerial.cpp \
    ./src/common/protoRouteTable.cpp \
    ./src/common/protoRouteMgr.cpp \
    ./src/common/protoQueue.cpp \
    ./src/common/protoPktRTP.cpp \
    ./src/common/protoPktIP.cpp \
    ./src/common/protoPktETH.cpp \
    ./src/common/protoPktARP.cpp \
    ./src/common/protoPkt.cpp \
    ./src/common/protoPipe.cpp \
    ./src/common/protoNet.cpp \
    ./src/common/protoList.cpp \
    ./src/common/protoLFSR.cpp \
    ./src/common/protoGraph.cpp \
    ./src/common/protoFile.cpp \
    ./src/common/protoDispatcher.cpp \
    ./src/common/protoDebug.cpp \
    ./src/common/protoChannel.cpp \
    ./src/common/protoCap.cpp \
    ./src/common/protoBitmask.cpp \
    ./src/common/protoBase64.cpp \
    ./src/common/protoApp.cpp \
    ./src/common/protoAddress.cpp

HEADERS += \
    ./include/manetGraph.h \
    ./include/wxProtoApp.h \
    ./include/protoVif.h \
    ./include/protoVersion.h \
    ./include/protoTree.h \
    ./include/protoTimer.h \
    ./include/protoTime.h \
    ./include/protoSpace.h \
    ./include/protoSocket.h \
    ./include/protoSerial.h \
    ./include/protoRouteTable.h \
    ./include/protoRouteMgr.h \
    ./include/protoQueue.h \
    ./include/protoPktRTP.h \
    ./include/protoPktIP.h \
    ./include/protoPktETH.h \
    ./include/protoPktARP.h \
    ./include/protoPkt.h \
    ./include/protoPipe.h \
    ./include/protoNet.h \
    ./include/protoList.h \
    ./include/protoLFSR.h \
    ./include/protokit.h \
    ./include/protoGraph.h \
    ./include/protoFile.h \
    ./include/protoDispatcher.h \
    ./include/protoDetour.h \
    ./include/protoDefs.h \
    ./include/protoDebug.h \
    ./include/protoChannel.h \
    ./include/protoCap.h \
    ./include/protoBitmask.h \
    ./include/protoBase64.h \
    ./include/protoApp.h \
    ./include/protoAddress.h \
    ./include/manetMsg.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE43C3D5D
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = protolib.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    SOURCES += ./src/linux/linuxRouteMgr.cpp \
        ./src/linux/linuxNet.cpp \
        ./src/unix/zebraRouteMgr.cpp \
	./src/unix/unixNet.cpp

    HEADERS += ./include/unix/zebraRouteMgr.h

    DEFINES += _FILE_OFFSET_BITS=64 \
    HAVE_ASSERT HAVE_DIRFD \
    HAVE_GETLOGIN HAVE_IPV6 \
    HAVE_LOCKF HAVE_OLD_SIGNALHANDLER \
    HAVE_SCHED \
    LINUX NO_SCM_RIGHTS PROTO_DEBUG UNIX

    INCLUDEPATH += ./include/unix

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

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog \
    README.maemo-meego
