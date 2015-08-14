#include "applogic.h"
#include <QtDeclarative>
#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    qmlRegisterType<AppLogic>("AppLogic", 1, 0, "AppLogic");

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/balls/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
