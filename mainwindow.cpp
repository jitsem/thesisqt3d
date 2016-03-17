#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QQmlEngine>
#include <QQmlContext>
#include <QMessageBox>
#include <QWidgetAction>
#include <QStandardPaths>
#include <QDebug>
#include <QCursor>
#include <QFileDialog>
#include <QDialog>
#include <QSpinBox>
#include <QDialogButtonBox>


#include "dragcomponent.h"
#include "calc.h"
#include "drawzone.h"
#include "component_lb.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QHBoxLayout *horlayout = new QHBoxLayout();
    ui->widget_container->setLayout(horlayout);
    QWidget *componentsWidget=new DragComponent();
    drawzoneWidget=new DrawZone(nullptr);
    componentsWidget->setMaximumWidth(100);
    horlayout->addWidget(componentsWidget);
    horlayout->addWidget(drawzoneWidget);

    calculator=Calc::Instance();

    //Connect signal to different classes
    QObject::connect (this,SIGNAL(on_actionRotate_triggered()),drawzoneWidget,SLOT(slotTriggeredRotate()));
    QObject::connect (this,SIGNAL(on_actionDelete_triggered()),drawzoneWidget,SLOT(slotTriggeredDelete()));
    QObject::connect (this,SIGNAL(on_actionSave_triggered()),drawzoneWidget,SLOT(slotTriggeredSave()));
    QObject::connect (this,SIGNAL(on_actionConnect_triggered()),drawzoneWidget,SLOT(slotTriggeredConnect()));



}

void MainWindow::delete3D()
{
    view->close();
    drawzoneWidget->drawCircuit();
    this->show();

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

void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Level"), QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation), tr("Levels (*.sj)"),0,QFileDialog::ReadOnly);

    if(!fileName.isEmpty()){

        auto temp = calculator->getFileName();
        calculator->setFileName(fileName);

        bool succes = calculator->readFile();
        if(succes){
            enableIcons();
            drawzoneWidget->drawCircuit();
        }
        else{
            calculator->setFileName(temp);
            QMessageBox msgBox;
            msgBox.setText("Error!");
            msgBox.setInformativeText("Invalid File!");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();
        }
    }
}

void MainWindow::on_action3D_Preview_triggered()
{
    //Check if cicuit is good
    if(drawzoneWidget->checkClosedCircuit()){

        drawzoneWidget->writeToVectors();

        this->hide();
        view = new QQuickView;
        view->engine()->rootContext()->setContextProperty(QStringLiteral("_window"), view);
        view->engine()->rootContext()->setContextProperty(QStringLiteral("mainWindow"),this);
        view->engine()->rootContext()->setContextProperty(QStringLiteral("calculator"),calculator.get());
        view->setResizeMode(QQuickView::SizeRootObjectToView);
        view->setSource(QUrl("qrc:/Qml/CircuitView.qml"));
        view->showFullScreen();
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Circuit not closed!");
        msgBox.setInformativeText("3D preview is not possible");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
    }

}

void MainWindow::on_actionNew_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Name new Level"), QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation), tr("Levels (*.sj)"));

    if(!fileName.isEmpty()){

        calculator->emptyVectors();
        calculator->setFileName(fileName);
        calculator->writeBackToFile();  //Write to create file. TODO check of nodig
        drawzoneWidget->drawCircuit();
        enableIcons();

    }
}

void MainWindow::on_actionEdit_Goals_triggered()
{
    //Make dialog for setting variables
    QDialog * d = new QDialog();
    QVBoxLayout * vbox = new QVBoxLayout();

    //Box for 3 start
    QLabel * label3Star = new QLabel("Number of clicks for 3 stars");
    QSpinBox * threeStarValue = new QSpinBox();
    threeStarValue->setRange(0,100);
    threeStarValue->setValue(calculator->getThreeStar());

    //Box for 3 start
    QLabel * label2Star = new QLabel("Number of clicks for 2 stars");
    QSpinBox * twoStarValue = new QSpinBox();
    twoStarValue->setRange(0,100);
    twoStarValue->setValue(calculator->getTwoStar());
    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                        | QDialogButtonBox::Cancel);




    QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));


    vbox->addWidget(label3Star);
    vbox->addWidget(threeStarValue);
    vbox->addWidget(label2Star);
    vbox->addWidget(twoStarValue);
    vbox->addWidget(buttonBox);

    d->setLayout(vbox);
    d->setWindowTitle("Edit Level Goals");

    int result = d->exec();
    if(result == QDialog::Accepted)
    {
        calculator->setThreeStar(threeStarValue->value());
        calculator->setTwoStar(twoStarValue->value());
    }

    //Clean up
    delete d,vbox,label3Star,threeStarValue,label2Star,twoStarValue,buttonBox;
}


void MainWindow::enableIcons(){

    //Enable some buttons
    ui->actionSave->setEnabled(true);
    ui->actionSave_as->setEnabled(true);
    ui->action3D_Preview->setEnabled(true);
    ui->actionConnect->setEnabled(true);
    ui->actionDelete->setEnabled(true);
    ui->actionRotate->setEnabled(true);
    ui->actionEdit_Goals->setEnabled(true);

    //Enable editor
    ui->widget_container->setEnabled(true);

}


