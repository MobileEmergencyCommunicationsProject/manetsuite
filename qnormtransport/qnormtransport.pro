#-------------------------------------------------
#
# Project created by QtCreator 2012-05-05T14:19:18
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = qnormtransport
TEMPLATE = lib
CONFIG += create_prl link_prl staticlib

DEFINES += QNORMTRANSPORT_LIBRARY

SOURCES += qnormtransport.cpp \
    norminstancemanager.cpp \
    normnotifier.cpp \
    qnormtransportstream.cpp \
    qnormfiletransport.cpp \
    normfileinfoiterator.cpp

HEADERS += qnormtransport.h\
        qnormtransport_global.h \
    norminstancemanager.h \
    normnotifier.h \
    qnormtransportstream.h \
    QNormTransport \
    QNormTransportStream \
    qnormfiletransport.h \
    QNormFileTransport \
    normfileinfoiterator.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7CFF4FF
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = qnormtransport.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    #
    # Every application that builds on the QNormTransport
    # classes must define these constants in order to compile
    # and run correctly.
    #
    DEFINES += _FILE_OFFSET_BITS=64 ECN_SUPPORT HAVE_ASSERT HAVE_DIRFD \
	HAVE_GETLOGIN HAVE_IPV6 HAVE_LOCKF \
	HAVE_OLD_SIGNALHANDLER LINUX PROTO_DEBUG UNIX

    maemo5 {
	target.path = /opt/manetsuite/lib
    } else:contains(MEEGO_EDITION,harmattan) {
	target.path = /opt/manetsuite/lib
    } else {
	target.path = /usr/lib
    }

    INSTALLS += target
}


INCLUDEPATH += $$PWD/../protolib/include
DEPENDPATH += $$PWD/../protolib

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/release/protolib.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/debug/protolib.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../protolib/libprotolib.a

INCLUDEPATH += $$PWD/../norm/include
DEPENDPATH += $$PWD/../norm

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../norm/release/norm.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../norm/debug/norm.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../norm/libnorm.a

