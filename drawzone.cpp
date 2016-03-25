#include "drawzone.h"
#include <QLabel>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include <memory>
#include <QPixmap>
#include <QDebug>
#include <QLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QApplication>
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
#include <vector>

#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QMessageBox>

#include "calc.h"
#include "source.h"
#include "resistor.h"
#include "wire.h"
#include "switch.h"
#include "ui_mainwindow.h"
#include "component_lb.h"



//TODO dynamically connect 2 components with drag select
//TODO make function showBox(QString) instead of repeating the messagebox code all the time

//Needed to determine move or click
QPoint  dragStartPosition;


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
    //DONE implement save from Calc class
    //DONE check if circuit that is drawn is correct
    //DONE change positions to positions in grid of 50 px
    //DONE fill out vectors of components
    //DONE execute filewriting method
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
        //TODO check if saving worked?

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
    //DONE make sure the input is tested!
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
    QList<component_lb*> list = this->findChildren<component_lb *>();
    int i=0;
    component_lb* gndwire;
    for(component_lb *w: list) {
        if(w->getSelected()){
            i++;
            if(w->getType()==2){
                gndwire=w;
                gndwire->setValue(COMPONENT_IS_GROUND);
            }
            else{
                //DONE make a messagebox
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
        component_lb *gnd = new component_lb(this,0,0,0,0,0,1,4);
        gnd->setFixedSize(MainWindow::Instance()->getGridSize(),MainWindow::Instance()->getGridSize());

        QPixmap grnd=QPixmap(":/assets/gnd.png");
        gnd->setPixmap(grnd);
        //TODO check if ground is put on top of other component

        if(gndwire->getAngle()==2 || gndwire->getAngle()==4){
            gnd->move(gndwire->x()-gndwire->width()/2+2*MainWindow::Instance()->getGridSize(),gndwire->y()+MainWindow::Instance()->getGridSize());
            polypoints[0]=QPoint(gndwire->x()+gndwire->width()/2,gndwire->y()+gndwire->height()/2);
            polypoints[1]=QPoint(gndwire->x()+2*MainWindow::Instance()->getGridSize(),gndwire->y()+gndwire->height()/2);
            polypoints[2]=QPoint(gndwire->x()+2*MainWindow::Instance()->getGridSize(),gndwire->y()+MainWindow::Instance()->getGridSize());
        }
        else{
            gnd->move(gndwire->x(),gndwire->y()+gndwire->height()+MainWindow::Instance()->getGridSize());
            polypoints[0]=QPoint(gndwire->x()+gndwire->width()/2,gndwire->y()+gndwire->height()/2);
            polypoints[1]=QPoint(gndwire->x()+gndwire->width()/2,gndwire->y()+gndwire->height()+MainWindow::Instance()->getGridSize());
            polypoints[2]=QPoint(gndwire->x()+gndwire->width()/2,gndwire->y()+gndwire->height()+MainWindow::Instance()->getGridSize());
        }
        gnd->setScaledContents(true);
        gnd->setAttribute(Qt::WA_DeleteOnClose);
        gnd->show();
        groundpresent=1;
        update();
    }
    else {
        //DONE make a messagebox
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
    component_lb *child = dynamic_cast<component_lb*>(childAt(dragStartPosition));
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
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength()
            < QApplication::startDragDistance())
        return;

    component_lb *child = dynamic_cast<component_lb*>(childAt(event->pos()));
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
    setCursor(Qt::OpenHandCursor);
    component_lb* gndWire;
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        //Do this only if the ground is present
        //For ground wire connection
        if(groundpresent){
            QList<component_lb*> list = this->findChildren<component_lb *>();
            for (component_lb * & w :list){
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

        component_lb *newIcon = new component_lb(this,value,0,0,0,0,angle,type,selected);
        newIcon->setScaledContents(true);
        newIcon->setFixedSize(MainWindow::Instance()->getGridSize(),MainWindow::Instance()->getGridSize());
        newIcon->setPixmap(pixmap);

        //Check if there is no component already there
        component_lb *child = nullptr;
        if (event->source()==this)
            child = dynamic_cast<component_lb*>(childAt(event->pos()));
        //TODO als het ding van de lijst komt de juiste positie voor child vinden
        //TODO IMPROVE!

        newIcon->move(event->pos() - offset);
        newIcon->setFixedSize(MainWindow::Instance()->getGridSize(),MainWindow::Instance()->getGridSize());
        newIcon->setScaledContents(true);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);
        newIcon->setFocusPolicy(Qt::StrongFocus);

        //DONE display values next to components
        updateNodePositions();

        //TODO add snaptogrid function
        //Move new component to nearest multiple of MainWindow::Instance()->getGridSize() pixels. (for every angle!!)
        if (newIcon->getType()==0 || newIcon->getType()==1){
            QLabel    *valueLabel = new QLabel(QString::number(newIcon->getValue()), this);
            valueLabel->setAttribute(Qt::WA_DeleteOnClose);
            QFont font;
            font=valueLabel->font();
            font.setPixelSize(MainWindow::Instance()->getGridSize()/4);
            valueLabel->setFont(font);
            switch (newIcon->getAngle()){
            case 1 :
                valueLabel->move(roundUp(newIcon->getNode1x(),MainWindow::Instance()->getGridSize()),roundUp(newIcon->getNode1y(),MainWindow::Instance()->getGridSize())-newIcon->height()/2);
                break;
            case 2:
                valueLabel->move(roundUp(newIcon->getNode1x(),MainWindow::Instance()->getGridSize())-(newIcon->width()/2),roundUp(newIcon->getNode2y(),MainWindow::Instance()->getGridSize()));
                break;
            case 3:(
                valueLabel->move(roundUp(newIcon->getNode2x(),MainWindow::Instance()->getGridSize()),roundUp(newIcon->getNode1y(),MainWindow::Instance()->getGridSize())-newIcon->height()/2));
                break;
            case 4:
                valueLabel->move(roundUp(newIcon->getNode1x(),MainWindow::Instance()->getGridSize())-(newIcon->width()/2),roundUp(newIcon->getNode1y(),MainWindow::Instance()->getGridSize()));
                break;
            }
            newIcon->setBuddy(valueLabel);
            valueLabel->show();
        }
        switch (newIcon->getAngle()){
        case 1 :
            newIcon->move(roundUp(newIcon->getNode1x(),MainWindow::Instance()->getGridSize()),roundUp(newIcon->getNode1y(),MainWindow::Instance()->getGridSize())-newIcon->height()/2);
            break;
        case 2:
            newIcon->move(roundUp(newIcon->getNode1x(),MainWindow::Instance()->getGridSize())-(newIcon->width()/2),roundUp(newIcon->getNode2y(),MainWindow::Instance()->getGridSize()));
            break;
        case 3:(
            newIcon->move(roundUp(newIcon->getNode2x(),MainWindow::Instance()->getGridSize()),roundUp(newIcon->getNode1y(),MainWindow::Instance()->getGridSize())-newIcon->height()/2));
            break;
        case 4:
            newIcon->move(roundUp(newIcon->getNode1x(),MainWindow::Instance()->getGridSize())-(newIcon->width()/2),roundUp(newIcon->getNode1y(),MainWindow::Instance()->getGridSize()));
            break;
        }
        updateNodePositions();

        //TODO the drawing of these connections points will be gone if you can connect two components!
        if (newIcon->getType()==4){
            gndWire->setFixedSize(MainWindow::Instance()->getGridSize(),MainWindow::Instance()->getGridSize());
            gndWire->setScaledContents(true);
            if(gndWire->getAngle()==2 || gndWire->getAngle()==4){
                polypoints[0]=QPoint(gndWire->x()+gndWire->width()/2,gndWire->y()+gndWire->height()/2);
                polypoints[1]=QPoint(newIcon->x()+newIcon->width()/2,newIcon->y()-MainWindow::Instance()->getGridSize());
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

            //TODO check if direction is oposite!
            //TODO make sure label is placed correctly if resistor/source is placed on top of another
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
    } else {
        event->ignore();
    }

}

//Keep dragstartposition
//Make sure everything is deselected when user clicks on empty space
void DrawZone::mousePressEvent(QMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
    component_lb *child = dynamic_cast<component_lb*>(childAt(event->pos()));
    if (!child){
        QList<component_lb*> list = this->findChildren<component_lb *>();
        for(component_lb *w : list) {
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

//Select component if releaseevent is near pressevent
void DrawZone::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    component_lb *child = dynamic_cast<component_lb*>(childAt(event->pos()));
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
    return;

}

component_lb *DrawZone::removeGray(component_lb &child){

    switch (child.getType()){

    case 0:
    {
        auto tempPixmap = std::make_shared<QPixmap>(":/assets/source_small.png");
        child.setPixmap((*tempPixmap));
        rotateToAngle(child);
    }
        break;
    case 1:
    {
        auto tempPixmap = std::make_shared<QPixmap>(":/assets/res_small.png");
        child.setPixmap((*tempPixmap));
        rotateToAngle(child);
    }
        break;
    case 2:
    {
        auto tempPixmap = std::make_shared<QPixmap>(":/assets/wire_small.png");
        child.setPixmap((*tempPixmap));
        rotateToAngle(child);
        break;
    }
    case 3:
    {
        auto tempPixmap = std::make_shared<QPixmap>(":/assets/sw_open.png");
        child.setPixmap((*tempPixmap));
        rotateToAngle(child);
        break;
    }
    case 4:
    {
        auto tempPixmap = std::make_shared<QPixmap>(":/assets/gnd.png");
        child.setPixmap((*tempPixmap));
        rotateToAngle(child);
        break;
    }
    default:
    {
        //should never get here
        break;
    }
    }
    child.setSelected(0);
    return &child;
}

component_lb *DrawZone::setGray(component_lb &child)
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
    QPainter painter(this);
    painter.setBrush(Qt::black);
    int i=0;
    int j=0;
    while(i<frameRect().width()){
        j=0;
        while(j<frameRect().height()){
            painter.drawRect(i,j,2,2);
            j+=MainWindow::Instance()->getGridSize();
        }
        i+=MainWindow::Instance()->getGridSize();
    }
    if(groundpresent){
        painter.drawPolyline(polypoints,3);
    }
    painter.end();
    return;
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
    QList<component_lb*> list = this->findChildren<component_lb *>();
    QList<QPoint> points;

    for(component_lb *w: list) {
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
            //TODO add alert to user
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


    QList<component_lb*> list = this->findChildren<component_lb *>();
    component_lb *current;
    for(component_lb *w: list){
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
    QList<component_lb*> stack;
    stack.push_back(current);
    QList<component_lb*> overschot=solveNode(list,current,curnode,stack);
    while (!overschot.isEmpty()){
        curnode++;
        current=overschot.takeFirst();
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
        for (component_lb* w : overschot) {
            if (!list.contains(w))
                list.append(w);
        }
        overschot=solveNode(list,current,curnode,stack);
    }
}

//Method for solving (filling out) one node
QList<component_lb *> DrawZone::solveNode(QList<component_lb *> &l,
                                          component_lb *current, int & curnode, QList<component_lb *> & stack){

    QList<component_lb*> halfaf;
    QList<component_lb*> neighbours;

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
                    for (component_lb* w: l) {
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
            component_lb * b=neighbours.takeFirst();
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
QList<component_lb *> DrawZone::getNeighbours(QList<component_lb *> &l, component_lb & current){

    QList<component_lb *> neighbours;
    for(component_lb *w:l){
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
    return neighbours;
}

//Converts from component_lb objects to vectorlists in 3d model
void DrawZone::writeToVectors()
{
    calc_nodes();
    QList<component_lb*> list = this->findChildren<component_lb *>();
    //fill out all drawn components in singleton calc object vectors
    std::shared_ptr<Calc> c = Calc::Instance();
    c->emptyVectors();
    for(component_lb *w : list){
        //TODO oplossen angles verkeerd
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
                                              w->getNode1x()/MainWindow::Instance()->getGridSize(),w->getNode1y()/MainWindow::Instance()->getGridSize(),angle,w->getAdjust(),w->getBegin(),
                                              w->getStepSize());
            auto wir = std::make_shared<Wire>(0.0,w->getNode1x()/MainWindow::Instance()->getGridSize(),w->getNode1y()/MainWindow::Instance()->getGridSize(),angle,1,w->getN2());
            c->addSource(s);
            c->addWire(wir);
            break;
        }
        case 1:
        {
            auto r = std::make_shared<Resistor>(w->getValue(),w->getN1(),w->getN2(),w->getNode1x()/MainWindow::Instance()->getGridSize(),w->getNode1y()/MainWindow::Instance()->getGridSize(),
                                                angle,w->getAdjust(),w->getBegin(),w->getStepSize());
            c->addResistor(r);
            break;
        }
        case 2:
        {
            auto wir = std::make_shared<Wire>(w->getValue(),w->getNode1x()/MainWindow::Instance()->getGridSize(),w->getNode1y()/MainWindow::Instance()->getGridSize(),angle,1,
                                              w->getN2(),0,w->getGoal());
            wir->setValue(w->getValue());
            c->addWire(wir);
            break;
        }
        case 3:
        {
            auto sw = std::make_shared<Switch>(w->getN1(),w->getN2(),w->getNode1x()/MainWindow::Instance()->getGridSize(),w->getNode1y()/MainWindow::Instance()->getGridSize(),angle);
            c->addSwitch(sw);
            break;
        }
        default :
            //TODO ground not added
            break;
        }
    }
}

//Circuit is drawn directly from file, from 3d vectors.. so conversion to component_lb objects is done here
void DrawZone::drawCircuit()
{
    //DONE vorige tekening clearen
    //TODO code ietwat verkleinen
    //DONE draad aan bron

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


    auto pixmap = std::make_shared<QPixmap>(":/assets/wire_small.png");
    for(auto w:wires){
        int XCoord =  w->getXCoord()*MainWindow::Instance()->getGridSize();
        int YCoord =  w->getYCoord()*MainWindow::Instance()->getGridSize();
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
                    YCoord -=MainWindow::Instance()->getGridSize()/2;
                else
                    XCoord +=MainWindow::Instance()->getGridSize();
                XCoord2 = XCoord +MainWindow::Instance()->getGridSize();
                YCoord2 = YCoord;

                break;
            case 2:
                if(i==0)
                    XCoord -=MainWindow::Instance()->getGridSize()/2;
                else
                    YCoord -= MainWindow::Instance()->getGridSize();
                YCoord2 = YCoord - MainWindow::Instance()->getGridSize();
                XCoord2 = XCoord;
                break;
            case 3:
                if(i==0)
                    YCoord -=MainWindow::Instance()->getGridSize()/2;
                else
                    XCoord -=MainWindow::Instance()->getGridSize();
                XCoord2 = XCoord -MainWindow::Instance()->getGridSize();
                YCoord2 = YCoord;
                break;
            case 4:
                if(i==0)
                    XCoord -=MainWindow::Instance()->getGridSize()/2;
                else
                    YCoord +=MainWindow::Instance()->getGridSize();
                YCoord2 = YCoord + MainWindow::Instance()->getGridSize();
                XCoord2 = XCoord;
                break;
            default:
                break;
            }

            component_lb *newIcon = new component_lb(this, w->getValue(), XCoord, YCoord, XCoord2,YCoord2,
                                                     angle, 2,0, w->getNode(),w->getNode(),0,0,0,w->getIsGoal());
            newIcon->setFixedSize(MainWindow::Instance()->getGridSize(),MainWindow::Instance()->getGridSize());
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

                component_lb *gnd = new component_lb(this,0,0,0,0,0,1,4);
                QPixmap grnd=QPixmap(":/assets/gnd.png");
                gnd->setPixmap(grnd);
                gnd->setFixedSize(MainWindow::Instance()->getGridSize(),MainWindow::Instance()->getGridSize());
                gnd->setScaledContents(true);
                if(newIcon->getAngle()==2 || newIcon->getAngle()==4){
                    gnd->move(newIcon->x()-newIcon->width()/2+MainWindow::Instance()->getGridSize()*2,newIcon->y()+MainWindow::Instance()->getGridSize());
                    polypoints[0]=QPoint(newIcon->x()+newIcon->width()/2,newIcon->y()+newIcon->height()/2);
                    polypoints[1]=QPoint(newIcon->x()+MainWindow::Instance()->getGridSize()*2,newIcon->y()+newIcon->height()/2);
                    polypoints[2]=QPoint(newIcon->x()+MainWindow::Instance()->getGridSize()*2,newIcon->y()+MainWindow::Instance()->getGridSize());
                }
                else{
                    gnd->move(newIcon->x(),newIcon->y()+newIcon->height()+MainWindow::Instance()->getGridSize()*2);

                    polypoints[0]=QPoint(newIcon->x()+newIcon->width()/2,newIcon->y()+newIcon->height()/2);
                    polypoints[1]=QPoint(newIcon->x()+newIcon->width()/2,newIcon->y()+newIcon->height()+MainWindow::Instance()->getGridSize()*2);
                    polypoints[2]=QPoint(newIcon->x()+newIcon->width()/2,newIcon->y()+newIcon->height()+MainWindow::Instance()->getGridSize()*2);
                }
                gnd->setAttribute(Qt::WA_DeleteOnClose);
                gnd->show();
                groundpresent=1;
                update();
            }
            //TODO if already component on the same spot, ignore or smth
            //DONE display values next to components
        }
    }

    pixmap = std::make_shared<QPixmap>(":/assets/source_small.png");
    for(auto s:sources){
        int XCoord =  s->getXCoord()*MainWindow::Instance()->getGridSize();
        int YCoord =  s->getYCoord()*MainWindow::Instance()->getGridSize();
        int XCoord2;
        int YCoord2;
        int angle = s->getAngle();

        QList<component_lb*> list = this->findChildren<component_lb *>();
        for(component_lb *w: list) {
            if(w->getNode1x()==XCoord && w->getNode1y()==YCoord && w->getAngle() == angle){
                delete w;
            }

        }

        switch(angle){
        case 1:

            YCoord -=MainWindow::Instance()->getGridSize()/2;
            XCoord2 = XCoord +MainWindow::Instance()->getGridSize();
            YCoord2 = YCoord;

            break;
        case 2:

            XCoord -=MainWindow::Instance()->getGridSize()/2;
            YCoord2 = YCoord + MainWindow::Instance()->getGridSize();
            XCoord2 = XCoord;

            break;
        case 3:

            YCoord -=MainWindow::Instance()->getGridSize()/2;
            XCoord2 = XCoord -MainWindow::Instance()->getGridSize();
            YCoord2 = YCoord;

            break;
        case 4:

            XCoord -=MainWindow::Instance()->getGridSize()/2;
            YCoord2 = YCoord - MainWindow::Instance()->getGridSize();
            XCoord2 = XCoord;

            break;
        default:
            break;
        }
        //TODO check if nodem en p are correct with node1 en node2
        component_lb *newIcon = new component_lb(this, s->getValue(), XCoord, YCoord, XCoord2,
                                                 YCoord2, angle,0,0, s->getNodep(),s->getNodem(),s->getIsAdjustable(),
                                                 s->getBeginValue(),s->getStepSize());
        newIcon->setFixedSize(MainWindow::Instance()->getGridSize(),MainWindow::Instance()->getGridSize());
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
        //TODO if already component on the same spot, ignore or smth
        //DONE display values next to components
        addValueToComponent(newIcon);

    }
    pixmap = std::make_shared<QPixmap>(":/assets/res_small.png");
    for(auto r:resistors){

        int XCoord =  r->getXCoord()*MainWindow::Instance()->getGridSize();
        int YCoord =  r->getYCoord()*MainWindow::Instance()->getGridSize();
        int XCoord2;
        int YCoord2;
        int angle = r->getAngle();

        switch(angle){
        case 1:

            YCoord -=MainWindow::Instance()->getGridSize()/2;
            XCoord2 = XCoord +MainWindow::Instance()->getGridSize();
            YCoord2 = YCoord;

            break;
        case 2:

            XCoord -=MainWindow::Instance()->getGridSize()/2;
            YCoord2 = YCoord + MainWindow::Instance()->getGridSize();
            XCoord2 = XCoord;

            break;
        case 3:

            YCoord -=MainWindow::Instance()->getGridSize()/2;
            XCoord2 = XCoord -MainWindow::Instance()->getGridSize();
            YCoord2 = YCoord;

            break;
        case 4:

            XCoord -=MainWindow::Instance()->getGridSize()/2;
            YCoord2 = YCoord - MainWindow::Instance()->getGridSize();
            XCoord2 = XCoord;

            break;
        default:
            break;
        }

        component_lb *newIcon = new component_lb(this, r->getValue(), XCoord, YCoord, XCoord2,YCoord2, angle, 1, 0, r->getNode1(),
                                                 r->getNode2(),r->getIsAdjustable(),r->getBeginValue(),r->getStepSize());
        newIcon->setFixedSize(MainWindow::Instance()->getGridSize(),MainWindow::Instance()->getGridSize());
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

        //DONE if already component on the same spot, ignore or smth
        //TODO display values next to components
        addValueToComponent(newIcon);


    }

    pixmap = std::make_shared<QPixmap>(":/assets/sw_open.png");
    for(auto s:switches){

        int XCoord =  s->getXCoord()*MainWindow::Instance()->getGridSize();
        int YCoord =  s->getYCoord()*MainWindow::Instance()->getGridSize();
        int XCoord2;
        int YCoord2;
        int angle = s->getAngle();

        switch(angle){
        case 1:

            YCoord -=MainWindow::Instance()->getGridSize()/2;
            XCoord2 = XCoord +MainWindow::Instance()->getGridSize();
            YCoord2 = YCoord;

            break;
        case 2:

            XCoord -=MainWindow::Instance()->getGridSize()/2;
            YCoord2 = YCoord + MainWindow::Instance()->getGridSize();
            XCoord2 = XCoord;

            break;
        case 3:

            YCoord -=MainWindow::Instance()->getGridSize()/2;
            XCoord2 = XCoord -MainWindow::Instance()->getGridSize();
            YCoord2 = YCoord;

            break;
        case 4:

            XCoord -=MainWindow::Instance()->getGridSize()/2;
            YCoord2 = YCoord - MainWindow::Instance()->getGridSize();
            XCoord2 = XCoord;

            break;
        default:
            break;
        }

        component_lb *newIcon = new component_lb(this, s->getValue(), XCoord, YCoord, XCoord2,YCoord2, angle, 3, 0,
                                                 s->getNode1(),s->getNode2());
        newIcon->setFixedSize(MainWindow::Instance()->getGridSize(),MainWindow::Instance()->getGridSize());
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


        //TODO if already component on the same spot, ignore or smth
        //DONE display values next to components

    }


}

//Create valuelabel that resizes to 1/4th of the gridsize
void DrawZone::addValueToComponent(component_lb * &newIcon){
    if(newIcon->getType()==0 || newIcon->getType()==1){
        QLabel    *valueLabel = new QLabel(QString::number(newIcon->getValue()), this);
        switch (newIcon->getAngle()){
        case 1 :
            valueLabel->move(roundUp(newIcon->getNode1x(),MainWindow::Instance()->getGridSize()),roundUp(newIcon->getNode1y(),MainWindow::Instance()->getGridSize())-newIcon->height()/2);
            break;
        case 2:
            valueLabel->move(roundUp(newIcon->getNode1x(),MainWindow::Instance()->getGridSize())-(newIcon->width()/2),roundUp(newIcon->getNode2y(),MainWindow::Instance()->getGridSize()));
            break;
        case 3:(
            valueLabel->move(roundUp(newIcon->getNode2x(),MainWindow::Instance()->getGridSize()),roundUp(newIcon->getNode1y(),MainWindow::Instance()->getGridSize())-newIcon->height()/2));
            break;
        case 4:
            valueLabel->move(roundUp(newIcon->getNode1x(),MainWindow::Instance()->getGridSize())-(newIcon->width()/2),roundUp(newIcon->getNode1y(),MainWindow::Instance()->getGridSize()));
            break;
        }
        newIcon->setBuddy(valueLabel);
        QFont font = valueLabel->font();
        font.setPixelSize(MainWindow::Instance()->getGridSize()/4);
        valueLabel->setFont(font);
        valueLabel->show();
    }
}

//See header
void DrawZone::rotateToAngle(component_lb &child){

    int curangle=child.getAngle();
    auto orig=std::make_shared<QPixmap>(*(child.pixmap()));
    QTransform transform;
    transform.rotate((-90*(curangle-1)));
    child.setPixmap(orig->transformed(transform));
}

//Take into account the angle of the components and fill out there nodal positions correctly
void DrawZone::updateNodePositions(){

    QList<component_lb*> list = this->findChildren<component_lb *>();
    for(component_lb *w: list) {
        switch (w->getAngle()){
        case 1:
            w->setNode1x(w->x());
            w->setNode1y(w->y()+(MainWindow::Instance()->getGridSize()/2));
            w->setNode2x(w->x()+MainWindow::Instance()->getGridSize());
            w->setNode2y(w->y()+(MainWindow::Instance()->getGridSize()/2));
            break;
        case 2:
            w->setNode1x(w->x()+(MainWindow::Instance()->getGridSize()/2));
            w->setNode1y(w->y()+(MainWindow::Instance()->getGridSize()));
            w->setNode2x(w->x()+(MainWindow::Instance()->getGridSize()/2));
            w->setNode2y(w->y());
            break;
        case 3:
            w->setNode1x(w->x()+MainWindow::Instance()->getGridSize());
            w->setNode1y(w->y()+(MainWindow::Instance()->getGridSize()/2));
            w->setNode2x(w->x());
            w->setNode2y(w->y()+(MainWindow::Instance()->getGridSize()/2));
            break;
        case 4:
            w->setNode1x(w->x()+(MainWindow::Instance()->getGridSize()/2));
            w->setNode1y(w->y());
            w->setNode2x(w->x()+(MainWindow::Instance()->getGridSize()/2));
            w->setNode2y(w->y()+MainWindow::Instance()->getGridSize());
            break;
        }
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
//Create interaction widgets/dialogs to edit components
void DrawZone::mouseDoubleClickEvent( QMouseEvent * event )
{
    if ( event->button() == Qt::LeftButton )
    {
        component_lb *child = static_cast<component_lb*>(childAt(event->pos()));
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

                //TODO check delete Clean up
                delete d;

                break;
            }
            case 2:
                QStringList sl;
                if(child->getGoal()==1)
                    sl << tr("yes") <<tr("no");
                else
                    sl << tr("no") <<tr("yes");
                QString item = QInputDialog::getItem(this,"tis voor aan te passen","Is this a goal",sl,0,false);
                if((!item.isEmpty()) && item == "yes"){
                    child->setGoal(1);
                }
                else
                    child->setGoal(0);
                break;

            }
        }
    }
}
