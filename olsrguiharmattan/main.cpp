#include "qmlapplicationviewer.h"
#include "qolsrapp.h"
#include <QtDeclarative>
#include <QtGui/QApplication>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QmlApplicationViewer viewer;
    QDeclarativeContext *context = viewer.rootContext();
    QOLSRApp applicationLogic;
    QObject *rootObject;

    qmlRegisterType<QOLSRApp>("QOLSRApp", 1, 0, "QOLSRApp");
    context->setContextProperty("applicationLogic", &applicationLogic);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/olsrguiharmattan/main.qml"));
    viewer.showExpanded();

    rootObject = (QObject*)viewer.rootObject();

    QObject::connect(rootObject, SIGNAL(updateRoutes()),
                     &applicationLogic, SLOT(onUpdateRoutes()));
    QObject::connect(rootObject, SIGNAL(updateNeighbors()),
                     &applicationLogic, SLOT(onUpdateNeighbors()));
    QObject::connect(rootObject, SIGNAL(settingsChanged(bool,bool,bool,double,double,double,double,double,double,double,double,double,double,double,double,int)),
                     &applicationLogic, SLOT(onSettingsChanged(bool,bool,bool,double,double,double,double,double,double,double,double,double,double,double,double,int)));
    QObject::connect(rootObject, SIGNAL(getSettings()),
                     &applicationLogic, SLOT(onGetSettings()));

    return app->exec();
}
