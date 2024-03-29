#include "drawzone.h"

#include <memory>
#include <vector>
#include <QLabel>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QLayout>
#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include <QInputDialog>
#include <QPoint>
#include <QMessageBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QDialog>
#include <QComboBox>
#include <QMainWindow>
#include <QWidget>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QScrollArea>
#include <QScroller>

#include "calc.h"
#include "source.h"
#include "resistor.h"
#include "wire.h"
#include "switch.h"
#include "ui_mainwindow.h"
#include "component_lb.h"
#include "component_lb.h"
#include "component.h"
#include "mainwindow.h"
#include "dragcomponent.h"



DrawZone::DrawZone(QWidget *parent)
    :QFrame(parent)
{
    setAcceptDrops(true);
    setAttribute( Qt::WA_PaintUnclipped, true );
    polypoints[0]=QPoint(0,0);
    polypoints[1]=QPoint(0,0);
    polypoints[2]=QPoint(0,0);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);

}

//Check if circuit is closed,
//Determine node numbers
//Convert from pixels to grid coordinates
//Convert to other model for 3d (to the vectors) We know we have to merge the models ;)
//Write to file
void DrawZone::slotTriggeredSave()
{

    if (!checkClosedCircuit()||!groundpresent){
        QMessageBox msgBox;
        if(!checkClosedCircuit())
            msgBox.setText("Circuit not closed!");
        else
            msgBox.setText("You have to ground a wire to complete the circuit");
        msgBox.setInformativeText("Saving is not possible");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    else {

        writeToVectors();
        std::shared_ptr<Calc> c = Calc::Instance();
        c->writeBackToFile();

        QMessageBox msgBox;
        msgBox.setModal(false);
        msgBox.setWindowFlags( Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint );
        msgBox.addButton(QMessageBox::Ok);
        msgBox.button(QMessageBox::Ok)->setText("saved succesfully");
        msgBox.button(QMessageBox::Ok)->animateClick(1000);
        msgBox.exec();
    }

}

//Add ground symbol to wire that is selected
void DrawZone::slotTriggeredGround()
{
    int gridSize = MainWindow::Instance()->getGridSize();

    if(groundpresent)
    {
        QMessageBox msgBox;
        msgBox.setModal(true);
        msgBox.setText("You can only ground a circuit once.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    QList<Component_lb*> list = this->findChildren<Component_lb *>();
    int i=0;
    Component_lb* gndwire;
    for(Component_lb *w: list) {
        if(w->getSelected()){
            i++;
            if(w->getType()==2){
                gndwire=w;
                gndwire->setValue(COMPONENT_IS_GROUND);
            }
            else{

                QMessageBox msgBox;
                msgBox.setModal(true);
                msgBox.setText("You can only ground a wire.");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                return;
            }
        }
    }
    if(i==1){

        updateNodePositions();
        Component_lb *gnd = new Component_lb(this,0,0,0,0,0,1,4);
        gnd->setFixedSize(gridSize,gridSize);

        QPixmap grnd=QPixmap(":/assets/icons/gnd.png");
        gnd->setPixmap(grnd);

        if(gndwire->getAngle()==2 || gndwire->getAngle()==4){
            gnd->move(gndwire->x()-gndwire->width()/2+2*gridSize,gndwire->y()+gridSize);
            polypoints[0]=QPoint(gndwire->x()+gndwire->width()/2,gndwire->y()+gndwire->height()/2);
            polypoints[1]=QPoint(gndwire->x()+2*gridSize,gndwire->y()+gndwire->height()/2);
            polypoints[2]=QPoint(gndwire->x()+2*gridSize,gndwire->y()+gridSize);
        }
        else{
            gnd->move(gndwire->x(),gndwire->y()+gndwire->height()+gridSize);
            polypoints[0]=QPoint(gndwire->x()+gndwire->width()/2,gndwire->y()+gndwire->height()/2);
            polypoints[1]=QPoint(gndwire->x()+gndwire->width()/2,gndwire->y()+gndwire->height()+gridSize);
            polypoints[2]=QPoint(gndwire->x()+gndwire->width()/2,gndwire->y()+gndwire->height()+gridSize);
        }
        gnd->setScaledContents(true);
        gnd->setAttribute(Qt::WA_DeleteOnClose);
        gnd->show();
        groundpresent=1;
        update();
    }
    else {

        QMessageBox msgBox;
        msgBox.setModal(true);
        msgBox.setText("You have to select only one wire to ground");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
}

//Nothing really happens, but needed to accept drops from our program
void DrawZone::dragEnterEvent(QDragEnterEvent *event)
{

    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

//For now only update polypoints to draw correct path when moving ground
//Will be updated for more components
void DrawZone::dragMoveEvent(QDragMoveEvent *event)
{
    Component_lb *child = dynamic_cast<Component_lb*>(childAt(dragStartPosition));
    if(child){
        if(event->source()==this){
            if (child->getType()==4){
                polypoints[1]=event->pos();
                polypoints[2]=event->pos();
                update();
            }
        }
        if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
            if (event->source() == this) {
                event->setDropAction(Qt::MoveAction);
                event->accept();
            } else {
                event->acceptProposedAction();
            }
        } else {
            event->ignore();
        }
    }
}

//Determine drag or select
//Execute dragging by creating QDrag obj
void DrawZone::mouseMoveEvent(QMouseEvent *event)
{


    if(selectedTool){
        //make sure only points that make rectangular corners are possible
        QPoint temp;
        temp=QPoint(roundUp(event->pos().x(),MainWindow::Instance()->getGridSize()),roundUp(event->pos().y(),MainWindow::Instance()->getGridSize()));
        if(!connectPoints.empty()){
            if(connectPoints.last().rx()==temp.rx()||connectPoints.last().ry()==temp.ry()){
                redDotPos=temp;
                update();
            }
        } else {
            redDotPos=(QPoint(roundUp(event->pos().x(),MainWindow::Instance()->getGridSize()),roundUp(event->pos().y(),MainWindow::Instance()->getGridSize())));
            update();
        }
    }
    if (!(event->buttons() & Qt::LeftButton)){
        return;
    }
    if ((event->pos() - dragStartPosition).manhattanLength()
            < QApplication::startDragDistance())
        return;

    Component_lb *child = dynamic_cast<Component_lb*>(childAt(event->pos()));
    if (!child)
        return;

    QWidget *b =child->buddy();
    if (b){
        b->close();
        delete b;
    }

    QPixmap pixmap = *child->pixmap();
    float value = child->getValue();
    int angle = child->getAngle();
    int type = child->getType();

    int selected =child->getSelected();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos() - child->pos())<< value << angle << type << selected;
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap.scaledToHeight(MainWindow::Instance()->getGridSize()));
    drag->setHotSpot(event->pos() - child->pos());

    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();

    child->setPixmap(tempPixmap);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {

        child->close();
        delete child;
    } else {
        child->setPixmap(pixmap);

    }
}

//Add new component to circuit when it is dropped
//Taking into account every sort of angle/grid size component_type,..
void DrawZone::dropEvent(QDropEvent *event)
{
    int gridSize = MainWindow::Instance()->getGridSize();
    Component_lb* gndWire;
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        //Do this only if the ground is present
        //For ground wire connection
        if(groundpresent){
            QList<Component_lb*> list = this->findChildren<Component_lb *>();
            for (Component_lb * & w :list){
                if (w->getValue()==COMPONENT_IS_GROUND){
                    gndWire=w;
                    break;
                }
            }
        }

        QPixmap pixmap;
        QPoint offset;
        float value;
        int angle ;
        int type ;
        int selected;
        dataStream >> pixmap >> offset >> value >> angle >> type >> selected;

        Component_lb *newIcon = new Component_lb(this,value,0,0,0,0,angle,type,selected);
        newIcon->setScaledContents(true);
        newIcon->setFixedSize(gridSize,gridSize);
        newIcon->setPixmap(pixmap);

        //Check if there is no component already there
        Component_lb *child = nullptr;
        if (event->source()==this)
            child = dynamic_cast<Component_lb*>(childAt(event->pos()));


        newIcon->move(event->pos() - offset);
        newIcon->setFixedSize(gridSize,gridSize);
        newIcon->setScaledContents(true);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);
        newIcon->setFocusPolicy(Qt::StrongFocus);


        updateNodePositions();

        //Move new component to nearest multiple of gridSize pixels. (for every angle!!)
        if (newIcon->getType()==0 || newIcon->getType()==1){
            QLabel    *valueLabel = new QLabel(QString::number(newIcon->getValue()), this);
            valueLabel->setAttribute(Qt::WA_DeleteOnClose);
            QFont font;
            font=valueLabel->font();
            font.setPixelSize(gridSize/4);
            valueLabel->setFont(font);
            switch (newIcon->getAngle()){
            case 1 :
                valueLabel->move(roundUp(newIcon->getNode1x(),gridSize),roundUp(newIcon->getNode1y(),gridSize)-newIcon->height()/2);
                break;
            case 2:
                valueLabel->move(roundUp(newIcon->getNode1x(),gridSize)-(newIcon->width()/2),roundUp(newIcon->getNode2y(),gridSize));
                break;
            case 3:(
                valueLabel->move(roundUp(newIcon->getNode2x(),gridSize),roundUp(newIcon->getNode1y(),gridSize)-newIcon->height()/2));
                break;
            case 4:
                valueLabel->move(roundUp(newIcon->getNode1x(),gridSize)-(newIcon->width()/2),roundUp(newIcon->getNode1y(),gridSize));
                break;
            }
            newIcon->setBuddy(valueLabel);
            valueLabel->show();
        }
        switch (newIcon->getAngle()){
        case 1 :
            newIcon->move(roundUp(newIcon->getNode1x(),gridSize),roundUp(newIcon->getNode1y(),gridSize)-newIcon->height()/2);
            break;
        case 2:
            newIcon->move(roundUp(newIcon->getNode1x(),gridSize)-(newIcon->width()/2),roundUp(newIcon->getNode2y(),gridSize));
            break;
        case 3:(
            newIcon->move(roundUp(newIcon->getNode2x(),gridSize),roundUp(newIcon->getNode1y(),gridSize)-newIcon->height()/2));
            break;
        case 4:
            newIcon->move(roundUp(newIcon->getNode1x(),gridSize)-(newIcon->width()/2),roundUp(newIcon->getNode1y(),gridSize));
            break;
        }
        updateNodePositions();


        if (newIcon->getType()==4){
            gndWire->setFixedSize(gridSize,gridSize);
            gndWire->setScaledContents(true);
            if(gndWire->getAngle()==2 || gndWire->getAngle()==4){
                polypoints[0]=QPoint(gndWire->x()+gndWire->width()/2,gndWire->y()+gndWire->height()/2);
                polypoints[1]=QPoint(newIcon->x()+newIcon->width()/2,newIcon->y()-gridSize);
                polypoints[2]=QPoint(newIcon->x()+newIcon->width()/2,newIcon->y());
            }
            else{
                polypoints[1]=QPoint(newIcon->x()+newIcon->width()/2,newIcon->y());
                polypoints[2]=QPoint(newIcon->x()+newIcon->width()/2,newIcon->y());
            }
            groundpresent=1;
            update();
        }

        if(child!=nullptr){


            if((newIcon->getNode1x()==child->getNode1x())&&(newIcon->getNode1y()==child->getNode1y())
                    &&(newIcon->getNode2x()==child->getNode2x())&&(newIcon->getNode2y()==child->getNode2y())){
                qDebug()<<"Placed component on top of other component";
                if(newIcon->buddy())
                    delete newIcon->buddy();
                delete newIcon;
                return;
            }
        }
        if (newIcon->getSelected()){
            setGray(*newIcon);
        }
        else{
            removeGray(*newIcon);
        }
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }


        //Re-adjust screen size
        adjustScreenSize();

    } else {
        event->ignore();
    }
}

//Keep dragstartposition
//Make sure everything is deselected when user clicks on empty space
void DrawZone::mousePressEvent(QMouseEvent *event)
{


    if(!selectedTool){
        Component_lb *child = dynamic_cast<Component_lb*>(childAt(event->pos()));
        if (!child){
            QList<Component_lb*> list = this->findChildren<Component_lb *>();
            for(Component_lb *w : list) {
                w->setSelected(0);
                MainWindow::Instance()->getUi()->actionRotate->setEnabled(false);
                MainWindow::Instance()->getUi()->actionDelete->setEnabled(false);
                removeGray(*w);
            }

            return;
        }

        if (event->button() == Qt::LeftButton){
            dragStartPosition = event->pos();

        }
    }

}

//Select component if releaseevent is near pressevent
void DrawZone::mouseReleaseEvent(QMouseEvent *event)
{


    if(selectedTool && (!doubleClicked)){
        if (!connectPoints.isEmpty()){
            QPoint temp=QPoint(roundUp(event->pos().x(),MainWindow::Instance()->getGridSize()),roundUp(event->pos().y(),MainWindow::Instance()->getGridSize()));
            if(connectPoints.last().rx()==temp.rx()||connectPoints.last().ry()==temp.ry()){
                connectPoints.push_back(temp);
                update();
            }
            if(connectPoints.size()>1){
                updateNodePositions();
                if(nodes.contains(connectPoints.last()))
                    connectComponents();
            }
            update();
        }
        else {
            connectPoints.push_back(QPoint(roundUp(event->pos().x(),MainWindow::Instance()->getGridSize()),roundUp(event->pos().y(),MainWindow::Instance()->getGridSize())));
            update();
        }
    }
    if (doubleClicked){
        doubleClicked=0;
    }
    else{

        Component_lb *child = dynamic_cast<Component_lb*>(childAt(event->pos()));
        if (!child)
            return;
        if ((event->pos() - dragStartPosition).manhattanLength()
                > QApplication::startDragDistance()){
            qDebug()<<"ignored move";
            return;
        }
        if(!(child->getSelected())){
            setGray(*child);
        }
        else{
            removeGray(*child);
        }
    }
    return;

}


Component_lb *DrawZone::removeGray(Component_lb &child){

    switch (child.getType()){

    case 0:
    {
        auto tempPixmap = std::make_shared<QPixmap>(":/assets/icons/source_small.png");
        child.setPixmap((*tempPixmap));
        rotateToAngle(child);
    }
        break;
    case 1:
    {
        auto tempPixmap = std::make_shared<QPixmap>(":/assets/icons/res_small.png");
        child.setPixmap((*tempPixmap));
        rotateToAngle(child);
    }
        break;
    case 2:
    {
        auto tempPixmap = std::make_shared<QPixmap>(":/assets/icons/wire_small.png");
        child.setPixmap((*tempPixmap));
        rotateToAngle(child);
        break;
    }
    case 3:
    {
        auto tempPixmap = std::make_shared<QPixmap>(":/assets/icons/sw_open.png");
        child.setPixmap((*tempPixmap));
        rotateToAngle(child);
        break;
    }
    case 4:
    {
        auto tempPixmap = std::make_shared<QPixmap>(":/assets/icons/gnd.png");
        child.setPixmap((*tempPixmap));
        rotateToAngle(child);
        break;
    }
    default:
    {
        //Should never get here
        break;
    }
    }
    child.setSelected(0);
    return &child;
}

Component_lb *DrawZone::setGray(Component_lb &child)
{
    QPixmap tempPixmap = *(child.pixmap());
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(child.pixmap()->rect(), QColor(127, 127, 127, 127));
    painter.end();
    child.setPixmap(tempPixmap);
    child.setSelected(1);

    MainWindow::Instance()->getUi()->actionRotate->setEnabled(true);
    MainWindow::Instance()->getUi()->actionDelete->setEnabled(true);

    return &child;
}

//Drawing a rectangle every gridsize pixels
//Also needs to draw connections between components if
void DrawZone::paintEvent(QPaintEvent *event)
{
    int gridSize = MainWindow::Instance()->getGridSize();
    QPainter painter(this);
    painter.setBrush(Qt::black);
    int i=0;
    int j=0;
    while(i<frameRect().width()){
        j=0;
        while(j<frameRect().height()){
            painter.drawEllipse(QPoint(i,j),gridSize/40,gridSize/40);
            j+=gridSize;
        }
        i+=gridSize;
    }
    if(groundpresent){
        painter.drawPolyline(polypoints,3);
    }
    if(selectedTool){
        if(!nodes.isEmpty()){
            painter.setBrush(Qt::green);
            for(QPoint a: nodes){
                painter.drawEllipse(a,gridSize/20,gridSize/20);
            }
        }
        if(redDotPos!=QPoint(0,0)){
            painter.setBrush(Qt::red);
            painter.drawEllipse(redDotPos,gridSize/20,gridSize/20);
        }
        for(QPoint a: connectPoints){
            painter.drawEllipse(a,gridSize/15,gridSize/15);
        }
        painter.setBrush(Qt::black);
        if(!connectPoints.isEmpty()){
            QPolygonF polyline;
            for(QPoint a: connectPoints){
                polyline.append(a);
            }
            polyline.append(redDotPos);
            painter.drawPolyline(polyline);

            painter.setBrush(Qt::blue);
            int i=0;
            while(i<frameRect().width()){
                painter.drawEllipse(QPoint(i,connectPoints.last().y()),gridSize/20,gridSize/20);
                i+=gridSize;
            }
            i=0;
            while(i<frameRect().height()){
                painter.drawEllipse(QPoint(connectPoints.last().x(),i),gridSize/20,gridSize/20);
                i+=gridSize;
            }
        }
    }

    painter.end();
    return;
}

void DrawZone::slotConnectChanged(bool b)
{
    auto m = MainWindow::Instance();
    selectedTool=b;

    /**Disable/Enable some functions*/
    setMouseTracking(b);
    setAcceptDrops(!b);
    m->getUi()->widget_container->findChild<DragComponent *>()->setEnabled(!b);
    m->toggleIcons(!b);



    if (!b){
        connectComponents();
        nodes.clear();
    }
    else {
        nodes.clear();
        updateNodePositions();
        update();
    }
}

int DrawZone::getGroundpresent() const
{
    return groundpresent;
}

void DrawZone::setGroundpresent(int value)
{
    groundpresent = value;
}

//Check circuit is closed counting connected points (node1&2,x&y)
bool DrawZone::checkClosedCircuit(){

    updateNodePositions();
    QList<Component_lb*> list = this->findChildren<Component_lb *>();
    QList<QPoint> points;

    for(Component_lb *w: list) {
        if(!(w->getType()==4)){
            points.push_back(QPoint(w->getNode1x(),w->getNode1y()));
            points.push_back(QPoint(w->getNode2x(),w->getNode2y()));
        }
    }

    //Take first element of list, check if it is in the rest of the list
    //If so, delete all occurances in the list and check next point
    //If some point only occurs one time in the list, the circuit is not closed (ROOM FOR IMPROVEMENT!!??)
    QPoint cur;
    while(!points.isEmpty()){
        cur=points.takeFirst();
        if(points.contains(cur)){
            while(points.contains(cur)){
                points.erase( std::remove( points.begin(), points.end(), cur), points.end() );
            }
        }
        else{
            qDebug()<<"Circuit not closed!";
            points.clear();
            return false;
        }
    }
    if(points.isEmpty()){
        qDebug()<<"tis aaneen";
        return true;

    }
}

//Main method that solves the node numbers in the circuit, calls solvenode for every node until all nodes are finished
//Backtracking algorithm that keeps a stack of visited components
void DrawZone::calc_nodes(){


    QList<Component_lb*> list = this->findChildren<Component_lb *>();
    Component_lb *current;
    for(Component_lb *w: list){
        w->setN1(-1);
        w->setN2(-1);
        if (w->getType()==2 && w->getValue()==COMPONENT_IS_GROUND){
            w->setN1(0);
            w->setN2(0);
            current=w;
            list.removeOne(w);
        }
        if(w->getType()==4){
            list.removeOne(w);
        }
    }
    int curnode=0;
    QList<Component_lb*> stack;
    stack.push_back(current);
    QList<Component_lb*> overschot=solveNode(list,current,curnode,stack);
    while (!overschot.isEmpty()){
        curnode++;
        for (Component_lb* o :overschot){
            if(o->getN1()!=-1 && o->getN2()!=-1){
                overschot.removeOne(o);
                list.removeOne(o);
            }
        }
        current=overschot.at(qrand() % overschot.size());
        overschot.removeOne(current);
        //current=overschot.takeFirst();
        if (current->getN1()==-1){
            current->setN1(curnode);
            if (current->getType()==2)
                current->setN2(curnode);
        }
        else if (current->getN2()==-1){
            current->setN2(curnode);
            if (current->getType()==2)
                current->setN1(curnode);
        }

        stack.push_back(current);
        for (Component_lb* w : overschot) {
            if (!list.contains(w))
                list.append(w);
        }
        overschot=solveNode(list,current,curnode,stack);
    }
}

//Method for solving (filling out) one node
QList<Component_lb *> DrawZone::solveNode(QList<Component_lb *> &l,
                                          Component_lb *current, int & curnode, QList<Component_lb *> & stack){

    QList<Component_lb*> halfaf;
    QList<Component_lb*> neighbours;

    while(!l.isEmpty()){

        neighbours = getNeighbours(l,*current);
        if(neighbours.isEmpty()){
            if(!stack.isEmpty()){
                current=(stack.takeLast());
            }
            else {//Empty stack means this node is solved

                if(!halfaf.isEmpty()){
                    return halfaf;
                }
                else{
                    int done =0;
                    for (Component_lb* w: l) {
                        if ((w->getN1()!=-1) && (w->getN2()!=-1))
                            done++;
                    }
                    if (done==l.size()){
                        return neighbours ;
                    }
                    else
                        return l;
                    break;
                }
            }

        }
        else if (!neighbours.isEmpty()){
            Component_lb * b=neighbours.takeFirst();
            if(b->getType()==2){
                b->setN1(curnode);
                b->setN2(curnode);
                stack.push_back(b);
                l.removeOne(b);
                current=b;
            }
            else {

                //Node that is the same as current gets same nodenr
                if((current->getNode2x()==b->getNode1x())&&(current->getNode2y()==b->getNode1y())){
                    b->setN1(curnode);
                }
                if((current->getNode1x()==b->getNode2x())&&(current->getNode1y()==b->getNode2y())){
                    b->setN2(curnode);
                }
                if((current->getNode1x()==b->getNode1x())&&(current->getNode1y()==b->getNode1y())){
                    b->setN1(curnode);
                }
                if((current->getNode2x()==b->getNode2x())&&(current->getNode2y()==b->getNode2y())){
                    b->setN2(curnode);
                }

                //Add other component than wire to halfaf
                if(!halfaf.contains(b)){
                    if ((b->getN1()!=-1)&&(b->getN2()!=-1)){
                        //Reeds af
                        l.removeOne(b);
                    }
                    else{
                        halfaf.append(b);
                    }
                }
                //Mark visited
                l.removeOne(b);

                if(!stack.isEmpty()){
                    current=stack.takeLast();
                }
                else{}//stack empty
            }
        }
    }//End while all cells are visited

    return halfaf;

}

//Get neighbouring components based on their nodes positions
QList<Component_lb *> DrawZone::getNeighbours(QList<Component_lb *> &l, Component_lb & current){

    QList<Component_lb *> neighbours;
    for(Component_lb *w:l){
        if(!(w==&current)){
            if((current.getNode2x()==w->getNode1x())&&(current.getNode2y()==w->getNode1y())){
                neighbours.append(w);
            }
            if((current.getNode1x()==w->getNode2x())&&(current.getNode1y()==w->getNode2y())){
                neighbours.append(w);
            }
            if((current.getNode1x()==w->getNode1x())&&(current.getNode1y()==w->getNode1y())){
                neighbours.append(w);
            }
            if((current.getNode2x()==w->getNode2x())&&(current.getNode2y()==w->getNode2y())){
                neighbours.append(w);
            }
        }
    }
    int i=0;
    for (Component_lb *n:neighbours){
        if(n->getType()==2){
            neighbours.prepend(neighbours.takeAt(i));
        }
    }
    return neighbours;
}

//Converts from component_lb objects to vectorlists in 3d model
void DrawZone::writeToVectors()
{
    int gridSize = MainWindow::Instance()->getGridSize();
    calc_nodes();
    QList<Component_lb*> list = this->findChildren<Component_lb *>();
    //Fill out all drawn components in singleton calc object vectors
    std::shared_ptr<Calc> c = Calc::Instance();
    c->emptyVectors();
    for(Component_lb *w : list){
        int angle = w->getAngle();
        if(angle == 4){
            angle = 2;
        }
        else if(angle == 2){
            angle = 4;
        }

        switch (w->getType()){

        case 0:
        {
            auto s = std::make_shared<Source>(w->getValue(),w->getN1(),w->getN2(),
                                              w->getNode1x()/gridSize,w->getNode1y()/gridSize,angle,w->getAdjust(),w->getBegin(),
                                              w->getStepSize());
            auto wir = std::make_shared<Wire>(0.0,w->getNode1x()/gridSize,w->getNode1y()/gridSize,angle,1,w->getN2());
            c->addSource(s);
            c->addWire(wir);
            break;
        }
        case 1:
        {
            auto r = std::make_shared<Resistor>(w->getValue(),w->getN1(),w->getN2(),w->getNode1x()/gridSize,w->getNode1y()/gridSize,
                                                angle,w->getAdjust(),w->getBegin(),w->getStepSize());
            c->addResistor(r);
            break;
        }
        case 2:
        {
            auto wir = std::make_shared<Wire>(w->getValue(),w->getNode1x()/gridSize,w->getNode1y()/gridSize,angle,1,
                                              w->getN2(),0,w->getVoltageGoal(),w->getCurrentGoal());
            wir->setValue(w->getValue());
            c->addWire(wir);
            break;
        }
        case 3:
        {
            auto sw = std::make_shared<Switch>(w->getN1(),w->getN2(),w->getNode1x()/gridSize,w->getNode1y()/gridSize,angle);
            c->addSwitch(sw);
            break;
        }
        default :
            break;
        }
    }
}

//Circuit is drawn directly from file, from 3d vectors.. so conversion to component_lb objects is done here
void DrawZone::drawCircuit()
{

    polypoints[0]=QPoint(0,0);
    polypoints[1]=QPoint(0,0);
    polypoints[2]=QPoint(0,0);
    update();

    int gridSize = MainWindow::Instance()->getGridSize();
    //Clear drawingfield
    QList<QWidget*> list = this->findChildren<QWidget *>();
    for(QWidget *w : list) {
        w->close();
        delete w;
    }

    std::shared_ptr<Calc> calculator=Calc::Instance();
    auto wires = calculator->getWires();
    auto sources = calculator->getSources();
    auto resistors = calculator->getResistors();
    auto switches = calculator->getSwitches();


    auto pixmap = std::make_shared<QPixmap>(":/assets/icons/wire_small.png");
    for(auto w:wires){
        int XCoord =  w->getXCoord()*gridSize;
        int YCoord =  w->getYCoord()*gridSize;
        int XCoord2;
        int YCoord2;
        int angle = w->getAngle();
        if(angle == 4){
            angle = 2;
        }
        else if(angle == 2){
            angle = 4;
        }
        for(int i = 0; i < w->getLength();i++){
            switch(angle){
            case 1:
                if(i==0)
                    YCoord -=gridSize/2;
                else
                    XCoord +=gridSize;
                XCoord2 = XCoord +gridSize;
                YCoord2 = YCoord;

                break;
            case 2:
                if(i==0)
                    XCoord -=gridSize/2;
                else
                    YCoord -= gridSize;
                YCoord2 = YCoord - gridSize;
                XCoord2 = XCoord;
                break;
            case 3:
                if(i==0)
                    YCoord -=gridSize/2;
                else
                    XCoord -=gridSize;
                XCoord2 = XCoord -gridSize;
                YCoord2 = YCoord;
                break;
            case 4:
                if(i==0)
                    XCoord -=gridSize/2;
                else
                    YCoord +=gridSize;
                YCoord2 = YCoord + gridSize;
                XCoord2 = XCoord;
                break;
            default:
                break;
            }

            Component_lb *newIcon = new Component_lb(this, w->getValue(), XCoord, YCoord, XCoord2,YCoord2,
                                                     angle, 2,0, w->getNode(),w->getNode(),0,0,0,w->getIsVoltageGoal(),w->getIsCurrentGoal());
            newIcon->setFixedSize(gridSize,gridSize);
            newIcon->setScaledContents(true);
            newIcon->setPixmap(*pixmap);
            if(angle == 1 || angle == 4)
                newIcon->move(QPoint(XCoord, YCoord));
            else
                newIcon->move(QPoint(XCoord2,YCoord2));
            rotateToAngle(*newIcon);
            newIcon->show();
            newIcon->setAttribute(Qt::WA_DeleteOnClose);
            newIcon->setFocusPolicy(Qt::StrongFocus);
            updateNodePositions();
            if (w->getValue()==COMPONENT_IS_GROUND){

                Component_lb *gnd = new Component_lb(this,0,0,0,0,0,1,4);
                QPixmap grnd=QPixmap(":/assets/icons/gnd.png");
                gnd->setPixmap(grnd);
                gnd->setFixedSize(gridSize,gridSize);
                gnd->setScaledContents(true);
                if(newIcon->getAngle()==2 || newIcon->getAngle()==4){
                    gnd->move(newIcon->x()-newIcon->width()/2+gridSize*2,newIcon->y()+gridSize);
                    polypoints[0]=QPoint(newIcon->x()+newIcon->width()/2,newIcon->y()+newIcon->height()/2);
                    polypoints[1]=QPoint(newIcon->x()+gridSize*2,newIcon->y()+newIcon->height()/2);
                    polypoints[2]=QPoint(newIcon->x()+gridSize*2,newIcon->y()+gridSize);
                }
                else{
                    gnd->move(newIcon->x(),newIcon->y()+newIcon->height()+gridSize*2);

                    polypoints[0]=QPoint(newIcon->x()+newIcon->width()/2,newIcon->y()+newIcon->height()/2);
                    polypoints[1]=QPoint(newIcon->x()+newIcon->width()/2,newIcon->y()+newIcon->height()+gridSize*2);
                    polypoints[2]=QPoint(newIcon->x()+newIcon->width()/2,newIcon->y()+newIcon->height()+gridSize*2);
                }
                gnd->setAttribute(Qt::WA_DeleteOnClose);
                gnd->show();
                groundpresent=1;
                update();
            }

        }

    }

    pixmap = std::make_shared<QPixmap>(":/assets/icons/source_small.png");
    for(auto s:sources){
        int XCoord =  s->getXCoord()*gridSize;
        int YCoord =  s->getYCoord()*gridSize;
        int XCoord2;
        int YCoord2;
        int angle = s->getAngle();
        if(angle==4){
            angle=2;
            YCoord -=gridSize;
        }
        else if (angle==2){
            angle=4;
            YCoord +=gridSize;
        }
        QList<Component_lb*> list = this->findChildren<Component_lb *>();
        for(Component_lb *w: list) {
            int ang=w->getAngle();
            int x = w->getNode1x();
            int y = w->getNode1y();
            if(ang==4){
                y +=gridSize;
            }
            else if (ang==2){
                y -= gridSize;
            }
            if(x==XCoord && y==YCoord && ang==angle){
                delete w;
            }

        }

        switch(angle){
        case 1:

            YCoord -=gridSize/2;
            XCoord2 = XCoord +gridSize;
            YCoord2 = YCoord;

            break;
        case 2:

            XCoord -=gridSize/2;
            YCoord2 = YCoord + gridSize;
            XCoord2 = XCoord;

            break;
        case 3:

            YCoord -=gridSize/2;
            XCoord2 = XCoord -gridSize;
            YCoord2 = YCoord;

            break;
        case 4:

            XCoord -=gridSize/2;
            YCoord2 = YCoord - gridSize;
            XCoord2 = XCoord;

            break;
        default:
            break;
        }
        Component_lb *newIcon = new Component_lb(this, s->getValue(), XCoord, YCoord, XCoord2,
                                                 YCoord2, angle,0,0, s->getNodep(),s->getNodem(),s->getIsAdjustable(),
                                                 s->getBeginValue(),s->getStepSize());
        newIcon->setFixedSize(gridSize,gridSize);
        newIcon->setScaledContents(true);
        newIcon->setPixmap(*pixmap);
        if(angle == 1 || angle == 2)

            newIcon->move(QPoint(XCoord, YCoord));
        else
            newIcon->move(QPoint(XCoord2,YCoord2));
        rotateToAngle(*newIcon);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);
        newIcon->setFocusPolicy(Qt::StrongFocus);

        updateNodePositions();

        addValueToComponent(newIcon);

    }
    pixmap = std::make_shared<QPixmap>(":/assets/icons/res_small.png");
    for(auto r:resistors){

        int XCoord =  r->getXCoord()*gridSize;
        int YCoord =  r->getYCoord()*gridSize;
        int XCoord2;
        int YCoord2;
        int angle = r->getAngle();

        switch(angle){
        case 1:

            YCoord -=gridSize/2;
            XCoord2 = XCoord +gridSize;
            YCoord2 = YCoord;

            break;
        case 2:

            XCoord -=gridSize/2;
            YCoord2 = YCoord + gridSize;
            XCoord2 = XCoord;

            break;
        case 3:

            YCoord -=gridSize/2;
            XCoord2 = XCoord -gridSize;
            YCoord2 = YCoord;

            break;
        case 4:

            XCoord -=gridSize/2;
            YCoord2 = YCoord - gridSize;
            XCoord2 = XCoord;

            break;
        default:
            break;
        }

        Component_lb *newIcon = new Component_lb(this, r->getValue(), XCoord, YCoord, XCoord2,YCoord2, angle, 1, 0, r->getNode1(),
                                                 r->getNode2(),r->getIsAdjustable(),r->getBeginValue(),r->getStepSize());
        newIcon->setFixedSize(gridSize,gridSize);
        newIcon->setScaledContents(true);
        newIcon->setPixmap(*pixmap);
        if(angle == 1 || angle == 2)
            newIcon->move(QPoint(XCoord, YCoord));
        else
            newIcon->move(QPoint(XCoord2,YCoord2));
        rotateToAngle(*newIcon);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);
        newIcon->setFocusPolicy(Qt::StrongFocus);


        updateNodePositions();

        addValueToComponent(newIcon);


    }

    pixmap = std::make_shared<QPixmap>(":/assets/icons/sw_open.png");
    for(auto s:switches){

        int XCoord =  s->getXCoord()*gridSize;
        int YCoord =  s->getYCoord()*gridSize;
        int XCoord2;
        int YCoord2;
        int angle = s->getAngle();

        switch(angle){
        case 1:

            YCoord -=gridSize/2;
            XCoord2 = XCoord +gridSize;
            YCoord2 = YCoord;

            break;
        case 2:

            XCoord -=gridSize/2;
            YCoord2 = YCoord + gridSize;
            XCoord2 = XCoord;

            break;
        case 3:

            YCoord -=gridSize/2;
            XCoord2 = XCoord -gridSize;
            YCoord2 = YCoord;

            break;
        case 4:

            XCoord -=gridSize/2;
            YCoord2 = YCoord - gridSize;
            XCoord2 = XCoord;

            break;
        default:
            break;
        }

        Component_lb *newIcon = new Component_lb(this, s->getValue(), XCoord, YCoord, XCoord2,YCoord2, angle, 3, 0,
                                                 s->getNode1(),s->getNode2());
        newIcon->setFixedSize(gridSize,gridSize);
        newIcon->setScaledContents(true);
        newIcon->setPixmap(*pixmap);
        if(angle == 1 || angle == 2)
            newIcon->move(QPoint(XCoord, YCoord));
        else
            newIcon->move(QPoint(XCoord2,YCoord2));
        rotateToAngle(*newIcon);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);
        newIcon->setFocusPolicy(Qt::StrongFocus);


        updateNodePositions();
        adjustScreenSize();


    }
}

//Create valuelabel that resizes to 1/4th of the gridsize
void DrawZone::addValueToComponent(Component_lb * &newIcon){

    int gridSize = MainWindow::Instance()->getGridSize();

    if(newIcon->getType()==0 || newIcon->getType()==1){
        QLabel    *valueLabel = new QLabel(QString::number(newIcon->getValue()), this);
        switch (newIcon->getAngle()){
        case 1 :
            valueLabel->move(roundUp(newIcon->getNode1x(),gridSize),roundUp(newIcon->getNode1y(),gridSize)-newIcon->height()/2);
            break;
        case 2:
            valueLabel->move(roundUp(newIcon->getNode1x(),gridSize)-(newIcon->width()/2),roundUp(newIcon->getNode2y(),gridSize));
            break;
        case 3:(
            valueLabel->move(roundUp(newIcon->getNode2x(),gridSize),roundUp(newIcon->getNode1y(),gridSize)-newIcon->height()/2));
            break;
        case 4:
            valueLabel->move(roundUp(newIcon->getNode1x(),gridSize)-(newIcon->width()/2),roundUp(newIcon->getNode1y(),gridSize));
            break;
        }
        newIcon->setBuddy(valueLabel);
        QFont font = valueLabel->font();
        font.setPixelSize(gridSize/4);
        valueLabel->setFont(font);
        valueLabel->show();
    }
}

//See header
void DrawZone::rotateToAngle(Component_lb &child){

    int curangle=child.getAngle();
    auto orig=std::make_shared<QPixmap>(*(child.pixmap()));
    QTransform transform;
    transform.rotate((-90*(curangle-1)));
    child.setPixmap(orig->transformed(transform));
}

//Take into account the angle of the components and fill out there nodal positions correctly
void DrawZone::updateNodePositions(){

    int gridSize = MainWindow::Instance()->getGridSize();

    QList<Component_lb*> list = this->findChildren<Component_lb *>();
    for(Component_lb *w: list) {
        switch (w->getAngle()){
        case 1:
            w->setNode1x(w->x());
            w->setNode1y(w->y()+(gridSize/2));
            w->setNode2x(w->x()+gridSize);
            w->setNode2y(w->y()+(gridSize/2));
            break;
        case 2:
            w->setNode1x(w->x()+(gridSize/2));
            w->setNode1y(w->y()+(gridSize));
            w->setNode2x(w->x()+(gridSize/2));
            w->setNode2y(w->y());
            break;
        case 3:
            w->setNode1x(w->x()+gridSize);
            w->setNode1y(w->y()+(gridSize/2));
            w->setNode2x(w->x());
            w->setNode2y(w->y()+(gridSize/2));
            break;
        case 4:
            w->setNode1x(w->x()+(gridSize/2));
            w->setNode1y(w->y());
            w->setNode2x(w->x()+(gridSize/2));
            w->setNode2y(w->y()+gridSize);
            break;
        }
        nodes.push_back(QPoint(w->getNode1x(),w->getNode1y()));
        nodes.push_back(QPoint(w->getNode2x(),w->getNode2y()));
    }
}

//Self explanatory, to snap to grid
int DrawZone::roundUp(int numToRound, int multiple)
{
    if(multiple == 0)
    {
        return numToRound;
    }
    int result;
    result = numToRound + multiple/2;
    result -= result % multiple;
    return result;

}
void DrawZone::addWire(QPoint &p,int dir){
    int gridsize=MainWindow::Instance()->getGridSize();
    auto pixmap = std::make_shared<QPixmap>(":/assets/icons/wire_small.png");
    Component_lb *newIcon = new Component_lb(this,0,p.x(),p.y(),0,0,dir,2);
    newIcon->setFixedSize(gridsize,gridsize);
    newIcon->setScaledContents(true);
    newIcon->setPixmap(*pixmap);
    rotateToAngle(*newIcon);
    switch (dir){
    case 1:
    {
        newIcon->move(QPoint(p.x(),p.y()-gridsize/2));
        break;
    }
    case 2:
    {
        newIcon->move(QPoint(p.x()-gridsize/2,p.y()-gridsize));
        break;
    }
    case 3:
    {
        newIcon->move(QPoint(p.x()-gridsize,p.y()-gridsize/2));
        break;
    }
    case 4:
    {
        newIcon->move(QPoint(p.x()-gridsize/2,p.y()));
        break;
    }

    }

    newIcon->show();
    newIcon->setAttribute(Qt::WA_DeleteOnClose);
    newIcon->setFocusPolicy(Qt::StrongFocus);
    updateNodePositions();
    update();
}

void DrawZone::adjustScreenSize()
{
    int gridsize=MainWindow::Instance()->getGridSize();
    QList<Component_lb*> list = this->findChildren<Component_lb *>();
    int tempx = 0;
    int tempy = 0;
    for(auto w : list){
        tempx = std::max(tempx,w->getNode1x());
        tempy = std::max(tempy,w->getNode1y());

    }
    //Adjust screen size
    this->setMinimumSize(tempx+gridsize,tempy+gridsize);
    this->parentWidget()->update();
}

void DrawZone::connectComponents(){

    int gridsize=MainWindow::Instance()->getGridSize();
    QPoint p,pEnd;
    if (!connectPoints.isEmpty())
        pEnd=connectPoints.last();
    while(!connectPoints.isEmpty()){
        p=connectPoints.first();
        connectPoints.removeOne(p);
        if (p==pEnd)
            break;
        if(p.y()==connectPoints.first().y()) //horizontal
        {
            if (p.x()<connectPoints.first().x())//to the right
            {
                addWire(p,1);
                if (((connectPoints.first().x()-p.x())/gridsize ) > 1){
                    connectPoints.push_front(QPoint(p.x()+gridsize,p.y()));
                    continue;
                }
            }
            else //to the left
            {
                addWire(p,3);
                if (((p.x()-connectPoints.first().x())/gridsize ) > 1){
                    connectPoints.push_front(QPoint(p.x()-gridsize,p.y()));
                    continue;
                }
            }
        }
        else if(p.x()==connectPoints.first().x())//vertical
        {
            if (p.y()<connectPoints.first().y())//down
            {
                addWire(p,4);
                if (((connectPoints.first().y()-p.y())/gridsize ) > 1){
                    connectPoints.push_front(QPoint(p.x(),p.y()+gridsize));
                    continue;
                }
            }
            else //up
            {
                addWire(p,2);
                if (((p.y()-connectPoints.first().y())/gridsize ) > 1){
                    connectPoints.push_front(QPoint(p.x(),p.y()-gridsize));
                    continue;
                }
            }

        }
    }
    connectPoints.clear();
    redDotPos=QPoint(0,0);
    update();
}

//Create interaction widgets/dialogs to edit components
void DrawZone::mouseDoubleClickEvent( QMouseEvent * event )
{
    if ( event->button() == Qt::LeftButton )
    {
        if(selectedTool){
            QPoint p2=event->pos();
            p2.setX(roundUp(p2.x(),MainWindow::Instance()->getGridSize()));
            p2.setY(roundUp(p2.y(),MainWindow::Instance()->getGridSize()));
            connectPoints.push_back(p2);
            connectComponents();
            doubleClicked=1;
        }
        else{
            Component_lb *child = static_cast<Component_lb*>(childAt(event->pos()));
            if (child!=nullptr){
                switch(child->getType()){

                case 0:
                case 1:{
                    //Make dialog for setting variables
                    QDialog * d = new QDialog();
                    QVBoxLayout * vbox = new QVBoxLayout();

                    //Box for value
                    QLabel * labelValue = new QLabel("Value of component?");
                    QDoubleSpinBox * value = new QDoubleSpinBox();
                    value->setRange(0,std::numeric_limits<float>::max());
                    value->setValue(child->getValue());

                    //Box for Adjustable
                    QLabel * labelAdjust = new QLabel("Is component adjustable?");
                    QCheckBox * adjust = new QCheckBox();

                    //Box for BeginValue
                    QLabel * labelBValue = new QLabel("Beginvalue of component?");
                    QDoubleSpinBox * bValue = new QDoubleSpinBox();
                    value->setRange(0,std::numeric_limits<float>::max());
                    bValue->setValue(child->getBegin());
                    bValue->setDisabled(true);

                    //Box for stepSize
                    QLabel * labelStep = new QLabel("StepSize of component?");
                    QDoubleSpinBox * step = new QDoubleSpinBox();
                    value->setRange(0,std::numeric_limits<float>::max());
                    step->setValue(child->getStepSize());
                    step->setDisabled(true);

                    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

                    QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
                    QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));
                    QObject::connect(adjust, SIGNAL(toggled(bool)), bValue, SLOT(setEnabled(bool)));
                    QObject::connect(adjust, SIGNAL(toggled(bool)), step, SLOT(setEnabled(bool)));

                    if(child->getAdjust()==1){
                        adjust->setChecked(true);
                    }else{
                        adjust->setChecked(false);
                    }

                    vbox->addWidget(labelValue);
                    vbox->addWidget(value);
                    vbox->addWidget(labelAdjust);
                    vbox->addWidget(adjust);
                    vbox->addWidget(labelBValue);
                    vbox->addWidget(bValue);
                    vbox->addWidget(labelStep);
                    vbox->addWidget(step);
                    vbox->addWidget(buttonBox);

                    d->setLayout(vbox);
                    d->setWindowTitle("Set Component Values");

                    int result = d->exec();
                    if(result == QDialog::Accepted)
                    {
                        child->setValue(value->value());
                        child->setAdjust(adjust->isChecked());
                        child->setBegin(bValue->value());
                        child->setStepSize(step->value());
                        dynamic_cast<QLabel*>(child->buddy())->setText(QString::number(value->value()));

                    }

                    //Clean up
                    delete d;

                    break;
                }
                case 2:
                    //Make dialog for setting variables
                    QDialog * d = new QDialog();
                    QVBoxLayout * vbox = new QVBoxLayout();

                    //Objects for voltage Goal
                    QLabel * labelVolt = new QLabel("Is there a VoltageGoal on this wire?");
                    QComboBox * cb = new QComboBox();
                    QStringList sl;
                    sl << tr("No") <<tr("Yes (Front of Wire)")<<tr("Yes (Back of Wire)");
                    cb->insertItems(0,sl);
                    cb->setCurrentIndex(child->getVoltageGoal());

                    //Objects for currentGoal
                    QLabel * labelCurr= new QLabel("Is there a CurrentGoal on this wire?");
                    QComboBox * cbc = new QComboBox();
                    QStringList slc;
                    slc << tr("No") <<tr("Yes");
                    cbc->insertItems(0,slc);
                    cbc->setCurrentIndex(child->getCurrentGoal());



                    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
                    QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
                    QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));

                    vbox->addWidget(labelVolt);
                    vbox->addWidget(cb);
                    vbox->addWidget(labelCurr);
                    vbox->addWidget(cbc);
                    vbox->addWidget(buttonBox);
                    d->setLayout(vbox);

                    int result = d->exec();
                    if(result == QDialog::Accepted)
                    {
                        child->setVoltageGoal(cb->currentIndex());
                        child->setCurrentGoal(cbc->currentIndex());

                    }
                    delete d;
                    break;

                }
            }
        }
    }
}
