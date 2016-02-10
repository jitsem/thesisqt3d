#include <QApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QQuickView>
#include <QQuickItem>
#include <QVariant>

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
using namespace std;

int main(int argc, char *argv[])
{


    //Dingen voor netwerkcalculation


    Calc* c=new Calc();

//    QFile f(":/assets/inputfile.txt");
//    std::vector<float> sol=c->solveLevel(&f);
//    std::stringstream ss;
//    for (int i=0;i<4;i++){
//    ss<<"voltage at node nr "<<i+1<<" "<<(sol.at(i))<<" volt"<<'\n';
//    }
//   std::string toPrint = ss.str();
//   QString str = QString::fromUtf8(toPrint.c_str());


   //Dingen voor 3D
   QApplication app(argc, argv);

   //default surface format -- moet altijd??
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
   view.setSource(QUrl("qrc:/Qml/AppView.qml"));


    QObject *text = view.rootObject();

   // text->setProperty("text",str);

    view.show();


       return app.exec();

}
