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
#include <set>


#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QMessageBox>

#include "calc.h"


component_lb * selected_comp ;
QPoint  dragStartPosition;



DrawZone::DrawZone(QWidget *parent)
    :QFrame(parent)
{

    setAcceptDrops(true);


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
    //TODO
    //DONE check if circuit that is drawn is correct
    //DONE change positions to positions in grid of 50 px
    //fill out vectors of components
    //execute filewriting method
    updateNodePositions();

    QList<component_lb*> list = this->findChildren<component_lb *>();
    QList<QPoint> points;

    foreach(component_lb *w, list) {
        //qDebug()<<"id:"<<w->getNr()<<"n1x, n1y"<< w->getNode1x()/50 << w->getNode1y()/50<<"n2x, n2y"<< w->getNode2x()/50 << w->getNode2y()/50<< "type:"<<w->getType();
        points.push_back(QPoint(w->getNode1x(),w->getNode1y()));
        points.push_back(QPoint(w->getNode2x(),w->getNode2y()));

    }
    //take first element of list, check if it is in the rest of the list
    //if so, delete all occurances in the list and check next point
    //if some point only occurs one time in the list, the circuit is not closed (ROOM FOR IMPROVEMENT!!??)
    //TODO if save is clicked when no components present.. give dialog
    QPoint cur;
    while(!points.isEmpty()){
        cur=points.takeFirst();
        if(points.contains(cur)){
            while(points.contains(cur)){
                points.erase( std::remove( points.begin(), points.end(), cur), points.end() );
            }
        }
        else{
            //qDebug()<<"Circuit not closed!";
            QMessageBox msgBox;
            msgBox.setText("Circuit not closed!");
            msgBox.setInformativeText("Do you want to save your changes? \n 3D preview will not be possible unless you close the circuit");
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            int ret = msgBox.exec();
            switch (ret) {

            case QMessageBox::Save:
                qDebug()<<"save was clicked";
                break;
            case QMessageBox::Cancel:
                qDebug()<<"cancel was clicked";
                break;
            default:
                // should never be reached
                break;

            }
            break;
        }
    }
    if(points.isEmpty()){
        qDebug()<<"tis aaneen";
        //fill out all drawn components in singleton calc object vectors
        //call the save function of calc class

        foreach(component_lb *w, list){


        }


    }
}



void DrawZone::slotTriggered3D_Preview()
{
    QQuickView *view = new QQuickView;
    std::shared_ptr<Calc> c=Calc::Instance();

    view->engine()->rootContext()->setContextProperty(QStringLiteral("_window"), view);
    view->engine()->rootContext()->setContextProperty(QStringLiteral("calculator"),c.get());
    view->setResizeMode(QQuickView::SizeRootObjectToView);

    view->setSource(QUrl("qrc:/Qml/CircuitView.qml"));

    QWidget *container = QWidget::createWindowContainer(view);


    container->show();



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
        child->show();
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
        //qDebug()<<"dropevent van deze component:" <<value<<angle<<type<<nr;


        component_lb *newIcon = new component_lb(this,value,0,0,0,0,angle,type,0,selected);
        newIcon->setPixmap(pixmap);
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
        updateNodePositions();

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



//    QList<component_lb*> list = this->findChildren<component_lb *>();
//    int i=0;
//    foreach(component_lb *w, list) {
//        i++;
//        qDebug()<<"element nr"<<i<<"id:"<<w->getNr()<<"'s position is :"<<w->x()<<w->y()<<"selected?:"<<w->getSelected()<<"type:"<<w->getType();
//    }

}

void DrawZone::mousePressEvent(QMouseEvent *event)
{

    //qDebug()<<"mousepressevent";

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
    //qDebug()<<"mousereleaseevent";
        return;


}
component_lb * DrawZone::removeGray(component_lb &child){

   // qDebug()<<child.getType();
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
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    QPainter painter(this);

    int i=0;
    int j=0;
    while(i<width()){
        j=0;
        while(j<height()){
           painter.drawRect(i,j,1,1);
           j+=50;
        }
        i+=50;
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
    if ( event->button() == Qt::LeftButton )
    {
        component_lb *child = static_cast<component_lb*>(childAt(event->pos()));
        if (child!=nullptr){
            child->setValue(QInputDialog::getDouble(this,"tis voor aan te passen","enter new value here",child->getValue(),-2147483647,2147483647,2));

        }
    }
}
