# Add more folders to ship with the application, here
folder_01.source = qml/balls
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE7E49BF3

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# Add dependency to Symbian components
# CONFIG += qt-components

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    applogic.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog

HEADERS += \
    applogic.h

unix:!macx:LIBS += $$OUT_PWD/../qnormtransport/libqnormtransport.a \
    $$OUT_PWD/../norm/libnorm.a \
    $$OUT_PWD/../protolib/libprotolib.a

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

DISTFILES += \
    README.md