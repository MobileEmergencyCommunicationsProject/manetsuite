#include <QDesktopServices>
#include <QtDeclarative/QDeclarativeContext>
#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QmlApplicationViewer viewer;
    QDeclarativeContext *context = viewer.rootContext();

    // Determine the documentsPath property for the
    // QML app.  The first search for files to share
    // begins in this directory.
    //
    // We try each of these reasonable choices in turn
    // until we find one defined for the platform:
    //
    // The user's documents directory
    // The users's desktop
    // The root directory
    //
    // The QML app might have to create the directory.
    // The existence of the definition does not
    // imply the existence of the directory.
    //
    QString documents = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    if (documents.isEmpty())
        documents = QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
    if (documents.isEmpty())
        documents = "/";

    context->setContextProperty("documentsPath", documents);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/mcshare/main.qml"));

    viewer.showExpanded();

    return app->exec();
}
