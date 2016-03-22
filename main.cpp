#include <QApplication>
#include <QQmlApplicationEngine>
#include <QObject>
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
#include "mainwindow.h"
using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Init calculations class
    std::shared_ptr<Calc> c=Calc::Instance();

    //Set some OpenGl parameters
    QSurfaceFormat format;
    if(QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL){
        format.setVersion(4,1);
        format.setProfile((QSurfaceFormat::CoreProfile));
    }
    format.setDepthBufferSize(24);
    format.setSamples(4);
    format.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);

    //Launch the MainWindow
    MainWindow w;
    w.show();





    return app.exec();

}
