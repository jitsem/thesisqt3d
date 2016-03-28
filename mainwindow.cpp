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
#include <QScreen>
#include <QWidget>
#include <QQuickView>


#include "dragcomponent.h"
#include "calc.h"
#include "drawzone.h"
#include "component_lb.h"



std::shared_ptr<MainWindow> MainWindow::instance=nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow)
{

    calculator=Calc::Instance();



}

void MainWindow::setUpUi()
{
    //Build basic ui
    ui->setupUi(this);

    //Calculate GridSize
    gridSize = QApplication::screens().at(0)->logicalDotsPerInch()/1.5;

    QHBoxLayout *horlayout = new QHBoxLayout();
    QWidget *componentsWidget=new DragComponent();
    drawzoneWidget=new DrawZone(this);

    ui->widget_container->setLayout(horlayout);
    horlayout->addWidget(componentsWidget);
    horlayout->addWidget(drawzoneWidget);


    componentsWidget->setMaximumWidth(gridSize*1.5);

    //Connect signal to different classes
    QObject::connect (this,SIGNAL(on_actionSave_triggered()),drawzoneWidget,SLOT(slotTriggeredSave()));
    QObject::connect (this,SIGNAL(on_actionGround_triggered()),drawzoneWidget,SLOT(slotTriggeredGround()));
}

int MainWindow::getGridSize() const
{
    return gridSize;
}

void MainWindow::setGridSize(int value)
{
    gridSize = value;
}

std::shared_ptr<MainWindow> MainWindow::Instance()
{
    //Return instance pointer; if it not exists, make it
    if(!instance){
        instance = std::shared_ptr<MainWindow>(new MainWindow()); //Like this because constructor is private
        instance->setUpUi();
    }

    return instance;
}


void MainWindow::delete3D()
{
    //Close 3d
    view->close();

    //Redraw circuit
    drawzoneWidget->drawCircuit();
    this->show();

}


MainWindow::~MainWindow()
{

    delete ui;
    exit(0);

}

void MainWindow::on_actionExit_triggered()
{
    //TODO CLEAN UP!
    exit(0);
}

void MainWindow::on_actionOpen_File_triggered()
{
    //Select a file open, if its a correct file: Use it to draw Circuit. Else display an error
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
            msgBox.setText("<b>Error!</b>");
            msgBox.setInformativeText("Invalid File!");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
    }
}

void MainWindow::on_action3D_Preview_triggered()
{
    //Check is circuit is valid, else launch an error
    if (!drawzoneWidget->checkClosedCircuit()||!drawzoneWidget->getGroundpresent()){
        QMessageBox msgBox;
        if(!drawzoneWidget->checkClosedCircuit())
            msgBox.setText("<b>Circuit not closed!</b>");
        else
            msgBox.setText("<b>You have to ground a wire to complete the circuit</b>");
        msgBox.setInformativeText("3D preview is not possible");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }

    //If circuit is valid, try to solve id. If it succeeds, launch 3D
    else {
        drawzoneWidget->writeToVectors();
        if(calculator->solveLevel()){

            this->hide();
            view = new QQuickView();

            //Pass some classes to the engine
            view->engine()->rootContext()->setContextProperty(QStringLiteral("_window"), view);
            view->engine()->rootContext()->setContextProperty(QStringLiteral("mainWindow"),this);
            view->engine()->rootContext()->setContextProperty(QStringLiteral("calculator"),calculator.get());
            view->setResizeMode(QQuickView::SizeRootObjectToView);
            view->setSource(QUrl("qrc:/Qml/CircuitView.qml"));
            view->showMaximized();
        }
        else{
            QMessageBox msgBox;
            msgBox.setText("<b>Sorry, Your application is a bit to complex for our engine</b>");
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
        if(!fileName.endsWith(".sj"))
            fileName.append(".sj");

        calculator->emptyVectors();
        calculator->setFileName(fileName);
        calculator->writeBackToFile();
        drawzoneWidget->setGroundpresent(0);
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



    //Signal/Slot for enabling boxes
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
    delete d;
}

void MainWindow::enableIcons(){

    //Enable some buttons
    ui->actionSave->setEnabled(true);
    ui->actionSave_as->setEnabled(true);
    ui->action3D_Preview->setEnabled(true);
    ui->actionGround->setEnabled(true);
    ui->actionEdit_Goals->setEnabled(true);

    //Enable editor
    ui->widget_container->setEnabled(true);

}



void MainWindow::on_actionRotate_triggered()
{

    QList<Component_lb*> list = drawzoneWidget->findChildren<Component_lb *>();
    for(auto w: list) {
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
    //Delete component(s), for some scenarios, delete some extra stuff
    QList<Component_lb*> list = drawzoneWidget->findChildren<Component_lb *>();
    for(auto w: list) {
        if(w->getSelected()){
            if(w->getType()==4){
                drawzoneWidget->setGroundpresent(0);
                drawzoneWidget->update();
                for(auto w2:list){
                    if(w2->getType()==2)
                        w2->setValue(0);
                }
            }
            if(!(w->buddy()==nullptr)){
                w->buddy()->close();
                delete w->buddy();
            }
            w->close();
            delete w;
        }

    }

}


void MainWindow::on_action_Copy_triggered()
{
    //Clear the clipboard, and put the selected components in
    copied.clear();
    QList<Component_lb*> list = drawzoneWidget->findChildren<Component_lb *>();
    for(auto w: list) {
        if (w->getSelected())
            copied.push_back(w);
    }

}

void MainWindow::on_action_Paste_triggered()
{
    //Put components if the clipboard on the drawzone
    if (!copied.isEmpty()){
        int i=0;
        for(auto w:copied){

            Component_lb *newIcon = new Component_lb(drawzoneWidget,w->getValue(),0,0,0,0,w->getAngle(),w->getType());
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


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("<b>Hello thanks for trying our app</b>");
    msgBox.setInformativeText("We made this application for our master thesis: 'Exploring the possibilities of Qt3D' \n\nFor more info on this topic, feel free to contact us!\n\n"
                              "Sander & Jitse");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::on_actionInfo_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("<b>Welcome to our app</b>");
    msgBox.setInformativeText("This levelBuilder is meant to be used in conjunction with the Squid Mayhem game of the other team.\n\n"
                              "To begin you will make a normal electronic circuit as is common with all electronics simulation programs. "
                              "At the moment, you can use the following components: Source, Resistor and Switch.\n"
                              "After you have chosen your components, connect them via wires and choose your ground. You can also chance some values by double-clicking the components.\n"
                              "Finally preview your work in 3D!\n\n"
                              "In our 3D world, voltage is represented by height, current is reprsesented by the size of the electrons and resistance by the size of the resistor.\n\n"
                              "Always remember to keep it simple: Will someone please think of the children!\n\n"
                              "Have fun playing with our app!");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}
