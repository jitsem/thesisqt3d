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
#include "component_lb.h"
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
#include <set>


#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QMessageBox>

#include "calc.h"
#include "source.h"
#include "resistor.h"
#include "wire.h"
#include "switch.h"
#include <vector>



//component_lb * selected_comp ;
QPoint  dragStartPosition;



DrawZone::DrawZone(QWidget *parent)
    :QFrame(parent)
{

    setAcceptDrops(true);
    setAttribute( Qt::WA_PaintUnclipped, true );
    width=this->size().width();
    height=size().height();
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    //setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);



}


void DrawZone::slotTriggeredRotate()
{

    QList<component_lb*> list = this->findChildren<component_lb *>();
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
void DrawZone::slotTriggeredDelete()
{
    QList<component_lb*> list = this->findChildren<component_lb *>();
    foreach(component_lb *w, list) {
        if(w->getSelected()){
            delete w;
        }

    }
}



void DrawZone::slotTriggeredSave()
{
    //TODO implement save from Calc class
    //DONE check if circuit that is drawn is correct
    //DONE change positions to positions in grid of 50 px
    //fill out vectors of components
    //execute filewriting method

    if(checkClosedCircuit()){
        writeToVectors();


    }else{

        QMessageBox msgBox;
        msgBox.setText("Circuit not closed!");
        msgBox.setInformativeText("3D preview will not be possible unless you close the circuit");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

    }
    std::shared_ptr<Calc> c = Calc::Instance();
    c->writeBackToFile();

}


void DrawZone::slotTriggeredConnect()
{
    //TODO expand to any two components!!
    //TODO make sure the input is tested!

    QList<component_lb*> list = this->findChildren<component_lb *>();
    int i=0;
    foreach(component_lb *w, list) {
        if(w->getSelected()){
            if(w->getType()==2){
                //keep w
                start=QPoint(w->getNode1x()+(w->width()/2),w->getNode1y()-25);
                w->setValue(COMPONENT_IS_GROUND);
                i++;
            }
            if(w->getType()==4){
                //keep ground
                stop=QPoint(w->getNode1x()+(w->width()/2),w->getNode1y()-25);
                i++;
            }

        }
    }
    if(i==2)
        connect=1;
    repaint();//or update()?
    return;
    //TODO hieronder is universele verbind functie.. denk dat we moeten werken met nodenummers van componenten om dit te laten functioneren
    //wilt zeggen dat de gebruiker een kan van de component aanklikt enz.
    //    updateNodePositions();
    //    QList<component_lb *> buren;
    //    int nr_selected = 0;
    //    foreach(component_lb *w, list) {
    //     if(w->getSelected()){
    //         buren.push_back(w);
    //         nr_selected++;
    //     }
    //    }
    //    if (nr_selected!=2){
    //        qDebug()<<"you have not selected 2 components";
    //    }
    //    else{
    //        //DONE connect two components in model
    //        //TODO connect the two components visually
    //      buren.at(0)->add_neighbour(buren.at(1));
    //      buren.at(1)->add_neighbour(buren.at(0));
    //        //TODO check again if two neigbours are correct
    //      //visual
    //      // add wire widgets because we need them in 3d..
    //        // how to determine corners?
    //      // or paint lines and add wire components to some list..?
    //        // how to keep them?
    //    }






}
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
void DrawZone::dragMoveEvent(QDragMoveEvent *event)
{
    // qDebug()<<"drag move event ";
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
void DrawZone::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength()
            < QApplication::startDragDistance())
        return;

    component_lb *child = static_cast<component_lb*>(childAt(event->pos()));
    if (!child)
        return;



    QPixmap pixmap = *child->pixmap();
    float value = (*child).getValue();
    int angle = (*child).getAngle();
    int type = (*child).getType();
    qint64 nr = (*child).getNr();
    int selected =(*child).getSelected();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos() - child->pos())<< value << angle << type <<nr << selected;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);


    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
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
        //child->show();
        child->setPixmap(pixmap);
    }




}
void DrawZone::dropEvent(QDropEvent *event)
{
    //qDebug()<<"dropevent position: "<<event->pos();
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        //qDebug()<<event->pos();
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);


        QPixmap pixmap;
        QPoint offset;
        float value;
        int angle ;
        int type ;
        qint64 nr;
        int selected;
        dataStream >> pixmap >> offset >> value >> angle >> type >> nr >> selected;


        component_lb *newIcon = new component_lb(this,value,0,0,0,0,angle,type,0,selected);
        newIcon->setPixmap(pixmap);
        //check if there is no component already there
        component_lb *child;
        if (event->source()==this)
            child = static_cast<component_lb*>(childAt(event->pos()));
        else
            //TODO als het ding van de lijst komt de juiste positie voor child vinden
            child = static_cast<component_lb*>(childAt(event->pos().rx()-100,event->pos().ry()));


        newIcon->move(event->pos() - offset);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);
        newIcon->setFocusPolicy(Qt::StrongFocus);

        //TODO if already component on the same spot, ignore or smth
        //DONE make sure the positions of the components are more logical, don't use left top point but relative to connection point
        //TODO display values next to components
        updateNodePositions();
        //move new component to nearest multiple of 50 pixels. (for every angle!!)
        switch (newIcon->getAngle()){
        case 1 :
            newIcon->move(roundUp(newIcon->getNode1x(),50),roundUp(newIcon->getNode1y(),50)-newIcon->height()/2);
            break;
        case 2:
            newIcon->move(roundUp(newIcon->getNode1x(),50)-(newIcon->width()/2),roundUp(newIcon->getNode2y(),50));
            break;
        case 3:(
            newIcon->move(roundUp(newIcon->getNode2x(),50),roundUp(newIcon->getNode1y(),50)-newIcon->height()/2));
            break;
        case 4:
            newIcon->move(roundUp(newIcon->getNode1x(),50)-(newIcon->width()/2),roundUp(newIcon->getNode1y(),50));
            break;
        }

        if (newIcon->getType()==4){
            QMessageBox msgBox;
            msgBox.setText("Please select the ground and the wire with select tool.");
            msgBox.exec();
        }
        updateNodePositions();
        QLabel    *valueLabel = new QLabel(QString::number(newIcon->getValue()), this);
        valueLabel->setBuddy(newIcon);
        valueLabel->show();
        if(child){
            //TODO check if direction is oposite!
            if((newIcon->getNode1x()==child->getNode1x())&&(newIcon->getNode1y()==child->getNode1y())&&(newIcon->getNode2x()==child->getNode2x())&&(newIcon->getNode2y()==child->getNode2y())){
                qDebug()<<"verkeerd gezet kut";
                delete newIcon;
                return;
            }
        }
        qDebug()<<newIcon->x()<<","<<newIcon->y();
        if (newIcon->getSelected()){
            setGray(*newIcon);
        }
        else{
            removeGray(*newIcon);
        }

        if (nr!=0){
            newIcon->setNr(nr);
        }
        else
            newIcon->setNr(qint64(newIcon));

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
void DrawZone::mousePressEvent(QMouseEvent *event)
{
    component_lb *child = static_cast<component_lb*>(childAt(event->pos()));
    if (!child){
        QList<component_lb*> list = this->findChildren<component_lb *>();
        foreach(component_lb *w, list) {
            w->setSelected(0);
            removeGray(*w);
        }
        return;
    }

    if (event->button() == Qt::LeftButton){
        dragStartPosition = event->pos();
    }
}
void DrawZone::mouseReleaseEvent(QMouseEvent *event)
{
    component_lb *child = static_cast<component_lb*>(childAt(event->pos()));
    if (!child)
        return;
    if (event->pos()!=dragStartPosition){
        qDebug()<<"ignored move";
        return;
    }
    if(!(child->getSelected())){
        child->setSelected(1);
        //kleur object grijs
        setGray(*child);
    }
    else{
        //haal grijze kleur weg
        removeGray(*child);
        child->setSelected(0);
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
        break;
    }
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
    return &child;
}
void DrawZone::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    int i=0;
    int j=0;
    //qDebug()<<frameRect().height();
    while(i<frameRect().width()){
        j=0;
        while(j<frameRect().height()){
            painter.drawRect(i,j,1,1);
            j+=50;
        }
        i+=50;
    }
    if(connect){
        painter.drawLine(start,stop);
        connect=0;
    }
    painter.end();
}
bool DrawZone::checkClosedCircuit(){
    updateNodePositions();

    QList<component_lb*> list = this->findChildren<component_lb *>();
    QList<QPoint> points;

    foreach(component_lb *w, list) {
        if(!(w->getType()==4)){
            points.push_back(QPoint(w->getNode1x(),w->getNode1y()));
            points.push_back(QPoint(w->getNode2x(),w->getNode2y()));
        }
    }
    //take first element of list, check if it is in the rest of the list
    //if so, delete all occurances in the list and check next point
    //if some point only occurs one time in the list, the circuit is not closed (ROOM FOR IMPROVEMENT!!??)
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
            return false;
            break;
        }
    }
    if(points.isEmpty()){
        qDebug()<<"tis aaneen";
        return true;

    }
}
void DrawZone::calc_nodes(){


    QList<component_lb*> list = this->findChildren<component_lb *>();
    component_lb *current;
    foreach(component_lb *w, list){
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
        foreach (component_lb* w, overschot) {
            if (!list.contains(w))
                list.append(w);
        }
        //if(list.isEmpty()){
        //   break;
        //}
        overschot=solveNode(list,current,curnode,stack);
    }
}

QList<component_lb *> DrawZone::solveNode(QList<component_lb *> &l, component_lb *current, int & curnode, QList<component_lb *> & stack){

    QList<component_lb*> halfaf;
    QList<component_lb*> neighbours;

    while(!l.isEmpty()){

        neighbours = getNeighbours(l,*current);
        if(neighbours.isEmpty()){
            if(!stack.isEmpty()){
                current=(stack.takeLast());
            }
            else {//empty stack means this node is solved

                if(!halfaf.isEmpty()){
                    return halfaf;
                }
                else{
                    int done =0;
                    foreach (component_lb* w, l) {
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
                //node that is the same as current gets same nodenr
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
                // add other component than wire to halfaf
                if(!halfaf.contains(b)){
                    //halfaf.append(b);
                    if ((b->getN1()!=-1)&&(b->getN2()!=-1)){
                        //reeds af
                        l.removeOne(b);
                    }
                    else{
                        halfaf.append(b);
                    }
                }
                //mark visited
                l.removeOne(b);

                if(!stack.isEmpty()){
                    current=stack.takeLast();
                }
                else{}//stack empty
            }
        }
    }//end while all cells are visited

    return halfaf;

}
QList<component_lb *> DrawZone::getNeighbours(QList<component_lb *> &l, component_lb & current){

    QList<component_lb *> neighbours;
    foreach(component_lb *w, l){
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
void DrawZone::writeToVectors()
{

    calc_nodes();
    QList<component_lb*> list = this->findChildren<component_lb *>();
    //fill out all drawn components in singleton calc object vectors
    std::shared_ptr<Calc> c = Calc::Instance();
    c->emptyVectors();
    foreach(component_lb *w, list){
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

            auto s = std::make_shared<Source>(w->getValue(),w->getN1(),w->getN2(),w->getNode1x()/50,w->getNode1y()/50,angle,w->getAdjust(),w->getBegin(),w->getStepSize());
            //Wire(int x, int y, int ang, int length, int node, float current=0.0);

            auto wir = std::make_shared<Wire>(0.0,w->getNode1x()/50,w->getNode1y()/50,angle,1,w->getN2());
            c->addSource(s);
            c->addWire(wir);
            break;
        }
        case 1:
        {
            auto r = std::make_shared<Resistor>(w->getValue(),w->getN1(),w->getN2(),w->getNode1x()/50,w->getNode1y()/50,angle,w->getAdjust(),w->getBegin(),w->getStepSize());
            c->addResistor(r);
            break;
        }
        case 2:
        {
            auto wir = std::make_shared<Wire>(w->getValue(),w->getNode1x()/50,w->getNode1y()/50,angle,1,w->getN2(),0,w->getGoal());
            wir->setValue(w->getValue());
            c->addWire(wir);
            break;
        }
        case 3:
        {
            //Switch::Switch(int np, int nm, int x, int y, int ang):
            auto sw = std::make_shared<Switch>(w->getN1(),w->getN2(),w->getNode1x()/50,w->getNode1y()/50,angle);
            c->addSwitch(sw);
            break;
        }
        default :
            //TODO ground not added
            break;
        }

    }
}
void DrawZone::drawCircuit()
{
    //TODO vorige tekening clearen
    //TODO code ietwat verkleinen
    //TODO draad aan bron

    //Clear drawingfield
    QList<component_lb*> list = this->findChildren<component_lb *>();
    foreach(component_lb *w, list) {
        delete w;
    }

    std::shared_ptr<Calc> calculator=Calc::Instance();
    auto wires = calculator->getWires();
    auto sources = calculator->getSources();
    auto resistors = calculator->getResistors();
    auto switches = calculator->getSwitches();


    auto pixmap = std::make_shared<QPixmap>(":/assets/wire_small.png");
    for(auto w:wires){
        int XCoord =  w->getXCoord()*50;
        int YCoord =  w->getYCoord()*50;
        int XCoord2;
        int YCoord2;
        int angle = w->getAngle();
        for(int i = 0; i < w->getLength();i++){
            switch(angle){
            case 1:
                if(i==0)
                    YCoord -=25;
                else
                    XCoord +=50;
                XCoord2 = XCoord +50;
                YCoord2 = YCoord;

                break;
            case 2:
                if(i==0)
                    XCoord -=25;
                else
                    YCoord += 50;
                YCoord2 = YCoord + 50;
                XCoord2 = XCoord;
                break;
            case 3:
                if(i==0)
                    YCoord -=25;
                else
                    XCoord -=50;
                XCoord2 = XCoord -50;
                YCoord2 = YCoord;
                break;
            case 4:
                if(i==0)
                    XCoord -=25;
                else
                    YCoord -=50;
                YCoord2 = YCoord - 50;
                XCoord2 = XCoord;
                break;
            default:
                break;
            }

            component_lb *newIcon = new component_lb(this, w->getValue(), XCoord, YCoord, XCoord2,YCoord2, angle, 2, 0, 0, w->getNode(),w->getNode(),0,0,0,w->getIsGoal());

            newIcon->setPixmap(*pixmap);
            if(angle == 1 || angle == 2)
                newIcon->move(QPoint(XCoord, YCoord));
            else
                newIcon->move(QPoint(XCoord2,YCoord2));
            rotateToAngle(*newIcon);
            newIcon->show();
            newIcon->setAttribute(Qt::WA_DeleteOnClose);
            newIcon->setFocusPolicy(Qt::StrongFocus);
            newIcon->setNr(qint64(newIcon));

            updateNodePositions();

            //TODO if already component on the same spot, ignore or smth
            //TODO display values next to components


        }
    }

    pixmap = std::make_shared<QPixmap>(":/assets/source_small.png");
    for(auto s:sources){



        int XCoord =  s->getXCoord()*50;
        int YCoord =  s->getYCoord()*50;
        int XCoord2;
        int YCoord2;
        int angle = s->getAngle();

        QList<component_lb*> list = this->findChildren<component_lb *>();
        foreach(component_lb *w, list) {
            if(w->getNode1x()==XCoord && w->getNode1y()==YCoord && w->getAngle() == angle){
                delete w;
            }

        }

        switch(angle){
        case 1:

            YCoord -=25;
            XCoord2 = XCoord +50;
            YCoord2 = YCoord;

            break;
        case 2:

            XCoord -=25;
            YCoord2 = YCoord + 50;
            XCoord2 = XCoord;

            break;
        case 3:

            YCoord -=25;
            XCoord2 = XCoord -50;
            YCoord2 = YCoord;

            break;
        case 4:

            XCoord -=25;
            YCoord2 = YCoord - 50;
            XCoord2 = XCoord;

            break;
        default:
            break;
        }
        //TODO check if nodem en p are correct with node1 en node2
        component_lb *newIcon = new component_lb(this, s->getValue(), XCoord, YCoord, XCoord2,YCoord2, angle, 0, 0, 0, s->getNodep(),s->getNodem(),s->getIsAdjustable(),s->getBeginValue(),s->getStepSize());

        newIcon->setPixmap(*pixmap);
        if(angle == 1 || angle == 2)

            newIcon->move(QPoint(XCoord, YCoord));
        else
            newIcon->move(QPoint(XCoord2,YCoord2));
        rotateToAngle(*newIcon);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);
        newIcon->setFocusPolicy(Qt::StrongFocus);
        newIcon->setNr(qint64(newIcon));

        updateNodePositions();

        //TODO if already component on the same spot, ignore or smth
        //TODO display values next to components


    }

    pixmap = std::make_shared<QPixmap>(":/assets/res_small.png");
    for(auto r:resistors){

        int XCoord =  r->getXCoord()*50;
        int YCoord =  r->getYCoord()*50;
        int XCoord2;
        int YCoord2;
        int angle = r->getAngle();

        switch(angle){
        case 1:

            YCoord -=25;
            XCoord2 = XCoord +50;
            YCoord2 = YCoord;

            break;
        case 2:

            XCoord -=25;
            YCoord2 = YCoord + 50;
            XCoord2 = XCoord;

            break;
        case 3:

            YCoord -=25;
            XCoord2 = XCoord -50;
            YCoord2 = YCoord;

            break;
        case 4:

            XCoord -=25;
            YCoord2 = YCoord - 50;
            XCoord2 = XCoord;

            break;
        default:
            break;
        }

        component_lb *newIcon = new component_lb(this, r->getValue(), XCoord, YCoord, XCoord2,YCoord2, angle, 1, 0, 0, r->getNode1(),r->getNode2(),r->getIsAdjustable(),r->getBeginValue(),r->getStepSize());

        newIcon->setPixmap(*pixmap);
        if(angle == 1 || angle == 2)
            newIcon->move(QPoint(XCoord, YCoord));
        else
            newIcon->move(QPoint(XCoord2,YCoord2));
        rotateToAngle(*newIcon);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);
        newIcon->setFocusPolicy(Qt::StrongFocus);
        newIcon->setNr(qint64(newIcon));

        updateNodePositions();

        //TODO if already component on the same spot, ignore or smth
        //TODO display values next to components


    }

    pixmap = std::make_shared<QPixmap>(":/assets/sw_open.png");
    for(auto s:switches){

        int XCoord =  s->getXCoord()*50;
        int YCoord =  s->getYCoord()*50;
        int XCoord2;
        int YCoord2;
        int angle = s->getAngle();

        switch(angle){
        case 1:

            YCoord -=25;
            XCoord2 = XCoord +50;
            YCoord2 = YCoord;

            break;
        case 2:

            XCoord -=25;
            YCoord2 = YCoord + 50;
            XCoord2 = XCoord;

            break;
        case 3:

            YCoord -=25;
            XCoord2 = XCoord -50;
            YCoord2 = YCoord;

            break;
        case 4:

            XCoord -=25;
            YCoord2 = YCoord - 50;
            XCoord2 = XCoord;

            break;
        default:
            break;
        }

        component_lb *newIcon = new component_lb(this, s->getValue(), XCoord, YCoord, XCoord2,YCoord2, angle, 3, 0, 0, s->getNode1(),s->getNode2());

        newIcon->setPixmap(*pixmap);
        if(angle == 1 || angle == 2)
            newIcon->move(QPoint(XCoord, YCoord));
        else
            newIcon->move(QPoint(XCoord2,YCoord2));
        rotateToAngle(*newIcon);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);
        newIcon->setFocusPolicy(Qt::StrongFocus);
        newIcon->setNr(qint64(newIcon));

        updateNodePositions();

        //TODO if already component on the same spot, ignore or smth
        //TODO display values next to components


    }

}
void DrawZone::rotateToAngle(component_lb &child){

    int curangle=child.getAngle();
    auto orig=std::make_shared<QPixmap>(*(child.pixmap()));
    QTransform transform;
    transform.rotate((-90*(curangle-1)));
    child.setPixmap(orig->transformed(transform));

}
void DrawZone::updateNodePositions(){

    QList<component_lb*> list = this->findChildren<component_lb *>();

    foreach(component_lb *w, list) {
        switch (w->getAngle()){
        case 1:
            // painter.drawRect(w->x(),w->y()+(w->height()/2),2,2);//node1
            // painter.drawRect(w->x()+w->width(),w->y()+(w->height()/2),2,2);//node2
            w->setNode1x(w->x());
            w->setNode1y(w->y()+(w->height()/2));
            w->setNode2x(w->x()+w->width());
            w->setNode2y(w->y()+(w->height()/2));
            break;
        case 2:
            //            painter.drawRect(w->x()+(w->width()/2),w->y()+w->height(),2,2);//node1
            //            painter.drawRect(w->x()+(w->width()/2),w->y(),2,2);//node2
            w->setNode1x(w->x()+(w->width()/2));
            w->setNode1y(w->y()+(w->height()));
            w->setNode2x(w->x()+(w->width()/2));
            w->setNode2y(w->y());
            break;
        case 3:
            //            painter.drawRect(w->x()+w->width(),w->y()+(w->height()/2),2,2);//node1
            //            painter.drawRect(w->x(),w->y()+(w->height()/2),2,2);//node2
            w->setNode1x(w->x()+w->width());
            w->setNode1y(w->y()+(w->height()/2));
            w->setNode2x(w->x());
            w->setNode2y(w->y()+(w->height()/2));
            break;
        case 4:
            //            painter.drawRect(w->x()+(w->width()/2),w->y(),2,2);//node1
            //            painter.drawRect(w->x()+(w->width()/2),w->y()+w->height(),2,2);//node2
            w->setNode1x(w->x()+(w->width()/2));
            w->setNode1y(w->y());
            w->setNode2x(w->x()+(w->width()/2));
            w->setNode2y(w->y()+w->height());
            break;
        }
    }
}
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
void DrawZone::mouseDoubleClickEvent( QMouseEvent * event )
{
    //Todo edit all values of source and resistor

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
                value->setRange(0,200000);
                value->setValue(child->getValue());

                //Box for Adjustable
                QLabel * labelAdjust = new QLabel("Is component adjustable?");
                QCheckBox * adjust = new QCheckBox();




                //Box for BeginValue
                QLabel * labelBValue = new QLabel("Beginvalue of component?");
                QDoubleSpinBox * bValue = new QDoubleSpinBox();
                bValue->setRange(0,200000);
                bValue->setValue(child->getBegin());
                bValue->setDisabled(true);

                //Box for stepSize
                QLabel * labelStep = new QLabel("StepSize of component?");
                QDoubleSpinBox * step = new QDoubleSpinBox();
                step->setRange(0,200000);
                step->setValue(child->getStepSize());
                step->setDisabled(true);

                QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                                    | QDialogButtonBox::Cancel);

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

                int result = d->exec();
                if(result == QDialog::Accepted)
                {
                    child->setValue(value->value());
                    child->setAdjust(adjust->isChecked());
                    child->setBegin(bValue->value());
                    child->setStepSize(step->value());
                }

                //Clean up
                delete d,vbox,value,bValue,step,adjust,buttonBox,labelAdjust,labelValue,labelBValue,labelStep;

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

