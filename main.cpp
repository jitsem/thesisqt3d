#include <QApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QQuickView>
#include <QQuickItem>
#include <QVariant>

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



    Calc* c=new Calc();
    QFile f(":/assets/inputfile.txt");


    std::vector<float> sol=c->solveLevel(&f);


    std::stringstream ss;

    for (int i=0;i<4;i++){
    ss<<"voltage at node nr "<<i+1<<" "<<(sol.at(i))<<" volt"<<'\n';
    }




   std::string toPrint = ss.str();
   QString str = QString::fromUtf8(toPrint.c_str());

    QApplication app(argc, argv);

    QQuickView view;
    view.setSource(QUrl(QStringLiteral("qrc:/Qml/AppView.qml")));


    QObject *text = view.rootObject();

    text->setProperty("text",str);

    view.show();


       return app.exec();

}
