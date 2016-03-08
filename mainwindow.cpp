#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QGraphicsItem>
#include "dragcomponent.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include "drawzone.h"
#include <QDebug>
#include <QCursor>
#include "component_lb.h"


#include <QQmlEngine>
#include <QQmlContext>
#include <QMessageBox>
#include <QWidgetAction>

#include "calc.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QHBoxLayout *horlayout = new QHBoxLayout(ui->widget_container);
    QWidget *components_widget=new QWidget();
    QWidget *drawzone_widget=new QWidget();
    components_widget->setMaximumWidth(100);
    horlayout->addWidget(components_widget);
    horlayout->addWidget(drawzone_widget);
    drawzone_widget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QVBoxLayout *verticalLayout = new QVBoxLayout(components_widget);
    verticalLayout->addWidget(new DragComponent(ui->widget_container));
    QGridLayout *gridlayout = new QGridLayout(drawzone_widget);
    DrawZone * drawzone =new DrawZone(ui->widget_container);
    gridlayout->addWidget(drawzone);
    // qDebug()<<drawzone_widget->size();
    QObject::connect (this,SIGNAL(on_actionRotate_triggered()),drawzone,SLOT(slotTriggeredRotate()));
    QObject::connect (this,SIGNAL(on_actionDelete_triggered()),drawzone,SLOT(slotTriggeredDelete()));
    QObject::connect (this,SIGNAL(on_actionSave_triggered()),drawzone,SLOT(slotTriggeredSave()));
    QObject::connect (this,SIGNAL(on_action3D_Preview_triggered()),this,SLOT(slotTriggered3D_Preview()));



}

void MainWindow::delete3D()
{
    view->close();
    this->show();

}

void MainWindow::slotTriggered3D_Preview()
{
    this->hide();
    std::shared_ptr<Calc> c=Calc::Instance();

    view = new QQuickView;
    view->engine()->rootContext()->setContextProperty(QStringLiteral("_window"), view);
    view->engine()->rootContext()->setContextProperty(QStringLiteral("mainWindow"),this);
    view->engine()->rootContext()->setContextProperty(QStringLiteral("calculator"),c.get());
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setSource(QUrl("qrc:/Qml/CircuitView.qml"));
    view->showFullScreen();

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_actionExit_triggered()
{
    //TODO CLEAN UP!
    exit(0);
}


