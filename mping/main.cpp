#include "normtest.h"
#include <QtDeclarative>
#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    qmlRegisterType<NormTest>("NormTest", 1, 0, "NormTest");

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/mping/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
