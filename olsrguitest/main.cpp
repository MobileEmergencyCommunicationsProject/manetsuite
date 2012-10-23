#include "olsrguitest.h"
#include <QtDeclarative>
#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    qmlRegisterType<OlsrGuiTest>("OlsrGuiTest", 1, 0, "OlsrGuiTest");

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/olsrguitest/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
