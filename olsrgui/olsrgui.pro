TEMPLATE = app
CONFIG += console
CONFIG -= qt




SYSTEM_LIBS = -ldl -lpthread

WX_FLAGS = `wx-config --cxxflags`
WX_LIBS = `wx-config --libs`

#WXCFLAGS = $$(WX_FLAGS)

LIBS += $$OUT_PWD/../protolib/libprotolib.a $$system(wx-config --libs) -ldl -lpthread

QMAKE_CXXFLAGS += $$system(wx-config --cxxflags)

SOURCES += \
    ../nrlolsr/common/nrlolsrgui.cpp \
    ../protolib/src/wx/wxProtoApp.cpp \
    ../protolib/src/linux/linuxCap.cpp

HEADERS += \
    ../nrlolsr/common/nrlolsrgui.h

unix {
    DEFINES += HAVE_ASSERT HAVE_DIRFD \
    HAVE_GETLOGIN HAVE_IPV6 \
    HAVE_LOCKF HAVE_OLD_SIGNALHANDLER \
    LINUX PROTO_DEBUG SMF_SUPPORT UNIX

#    INCLUDEPATH += ../protolib/include/unix

#    maemo5 {
#	target.path = /opt/manetsuite/bin
#    } else:contains(MEEGO_EDITION,harmattan) {
#	target.path = /opt/manetsuite/bin
#    } else {
	target.path = /usr/bin
#    }

    INSTALLS += target

#    SOURCES += ../protolib/src/linux/linuxCap.cpp
}


INCLUDEPATH += $$PWD/../protolib/include
DEPENDPATH += $$PWD/../protolib

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/release/protolib.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../protolib/debug/protolib.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../protolib/libprotolib.a

