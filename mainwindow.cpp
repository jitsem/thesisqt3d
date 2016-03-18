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




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QHBoxLayout *horlayout = new QHBoxLayout();
    ui->widget_container->setLayout(horlayout);
    QWidget *componentsWidget=new DragComponent();
    drawzoneWidget=new DrawZone(this);
    componentsWidget->setMaximumWidth(100);
    horlayout->addWidget(componentsWidget);
    horlayout->addWidget(drawzoneWidget);

    calculator=Calc::Instance();

    //Connect signal to different classes
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

    if (!drawzoneWidget->checkClosedCircuit()||!drawzoneWidget->getGroundpresent()){
        QMessageBox msgBox;
        if(!drawzoneWidget->checkClosedCircuit())
            msgBox.setText("Circuit not closed!");
        else
            msgBox.setText("You have to ground a wire to complete the circuit");
        msgBox.setInformativeText("3D preview is not possible");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    else {

        drawzoneWidget->writeToVectors();
        if(calculator->solveLevel()){

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
            msgBox.setText("Sorry, Your application is a bit to complex for our engine");
            msgBox.setInformativeText("Please refrain from shorting out components");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();


        }
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
    ui->actionEdit_Goals->setEnabled(true);

    //Enable editor
    ui->widget_container->setEnabled(true);

}



void MainWindow::on_actionRotate_triggered()
{

    QList<component_lb*> list = drawzoneWidget->findChildren<component_lb *>();
    foreach(component_lb *w, list) {
        if(w->getSelected()){
            auto orig=std::make_shared<QPixmap>(*(w->pixmap()));
            QTransform transform;
            transform.rotate(-90);
            w->setPixmap(orig->transformed(transform));
            if (w->getAngle()==4){
                w->setAngle(1);
            }
            else
                w->setAngle((w->getAngle())+1);

        }

    }

}

Ui::MainWindow *MainWindow::getUi() const
{
    return ui;
}

void MainWindow::on_actionDelete_triggered()
{

    QList<component_lb*> list = drawzoneWidget->findChildren<component_lb *>();
    foreach(component_lb *w, list) {
        if(w->getSelected()){
            if(w->getType()==4){
                drawzoneWidget->setGroundpresent(0);
                drawzoneWidget->update();
            }
            if(!(w->buddy()==nullptr)){
                w->buddy()->close();
                delete w->buddy();
            }
            if (w->getValue()==COMPONENT_IS_GROUND)
                w->setValue(0);
            w->close();
            delete w;
        }

    }

}


void MainWindow::on_action_Copy_triggered()
{
        copied.clear();
        QList<component_lb*> list = drawzoneWidget->findChildren<component_lb *>();
        foreach(component_lb *w, list) {
            if (w->getSelected())
                copied.push_back(w);
        }

}

void MainWindow::on_action_Paste_triggered()
{
    if (!copied.isEmpty()){
        int i=0;
        foreach(component_lb* w,copied){

            component_lb *newIcon = new component_lb(drawzoneWidget,w->getValue(),0,0,0,0,w->getAngle(),w->getType());
            newIcon->setPixmap(*(w->pixmap()));
            newIcon->move(QPoint(600+i*50,50));
            newIcon->show();
            newIcon->setAttribute(Qt::WA_DeleteOnClose);
            newIcon->setFocusPolicy(Qt::StrongFocus);
            drawzoneWidget->updateNodePositions();
            i++;

        }
    }
}
