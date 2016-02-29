#include <QApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QQuickView>
#include <QQuickItem>
#include <QVariant>
#include <QDebug>

#include <QQmlContext>
#include <QQmlEngine>
#include <QOpenGLContext>
#include <QSurfaceFormat>


#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <typeinfo>

#include "source.h"
#include "resistor.h"
#include "calc.h"
#include "component.h"
using namespace std;

int main(int argc, char *argv[])
{


    //Dingen voor netwerkcalculation
    Calc* c=new Calc();





   //Dingen voor 3D
   QApplication app(argc, argv);
   QSurfaceFormat format;
   if(QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL){
       format.setVersion(4,1);
       format.setProfile((QSurfaceFormat::CoreProfile));
   }
   format.setDepthBufferSize(24);
   format.setSamples(4);
   format.setStencilBufferSize(8);
   QSurfaceFormat::setDefaultFormat(format);


   //load view
   QQuickView view;
   view.engine()->rootContext()->setContextProperty(QStringLiteral("_window"), &view);
   view.engine()->rootContext()->setContextProperty(QStringLiteral("calculator"),c);
   view.setResizeMode(QQuickView::SizeRootObjectToView);
   view.setSource(QUrl("qrc:/Qml/CircuitView.qml"));



    view.show();


       return app.exec();

}
