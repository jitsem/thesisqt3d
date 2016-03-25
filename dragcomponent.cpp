#include "dragcomponent.h"


#include <memory>
#include <QLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include <QPixmap>
#include <QGraphicsItem>
#include <QDebug>
#include <QWidget>

#include "component_lb.h"
#include "mainwindow.h"


DragComponent::DragComponent(QWidget *parent)
    :QFrame(parent)
{

    //TODO clean up pointers in destructor
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(true);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->setAlignment(Qt::AlignTop);
    QWidgetList components;

    Component_lb *sourceIcon = new Component_lb(this,15,0,0,0,0,1,0);
    QPixmap source=QPixmap(":/assets/source_small.png");
    sourceIcon->setPixmap(source);
    sourceIcon->setFixedSize(MainWindow::Instance()->getGridSize(),MainWindow::Instance()->getGridSize());
    sourceIcon->setScaledContents(true);
    sourceIcon->setAttribute(Qt::WA_DeleteOnClose);
    sourceIcon->show();


    Component_lb *res1 = new Component_lb(this,100,3,4,20,4,1,1);
    QPixmap res2=QPixmap(":/assets/res_small.png");
    res1->setPixmap(res2);
    res1->move(0,10);
    res1->setFixedSize(MainWindow::Instance()->getGridSize(),MainWindow::Instance()->getGridSize());
    res1->setScaledContents(true);
    res1->setAttribute(Qt::WA_DeleteOnClose);
    res1->show();

    Component_lb *wire1 = new Component_lb(this,0,3,4,20,4,1,2);
    QPixmap wir=QPixmap(":/assets/wire_small.png");
    wire1->setPixmap(wir);
    wire1->setFixedSize(MainWindow::Instance()->getGridSize(),MainWindow::Instance()->getGridSize());
    wire1->setScaledContents(true);
    wire1->setAttribute(Qt::WA_DeleteOnClose);
    wire1->show();


    Component_lb *sw_op = new Component_lb(this,0,0,0,0,0,1,3);
    QPixmap switch_open=QPixmap(":/assets/sw_open.png");
    sw_op->setPixmap(switch_open);
    sw_op->setFixedSize(MainWindow::Instance()->getGridSize(),MainWindow::Instance()->getGridSize());
    sw_op->setScaledContents(true);
    sw_op->setAttribute(Qt::WA_DeleteOnClose);
    sw_op->show();


    //foreach component
    components.append(sourceIcon);
    components.append(res1);
    components.append(wire1);
    components.append(sw_op);
    mainLayout->addWidget(components[0]);
    mainLayout->addWidget(components[1]);
    mainLayout->addWidget(components[2]);
    mainLayout->addWidget(components[3]);

}

void DragComponent::dragEnterEvent(QDragEnterEvent *event)
{
        //qDebug()<<"dragenterEvent from the list";
        event->ignore();

}

void DragComponent::dragMoveEvent(QDragMoveEvent *event)
{
    //this event never occurs because it is not called by the program, is kept for now for info
        event->ignore();
}

void DragComponent::dropEvent(QDropEvent *event)
{
return;
}

void DragComponent::mousePressEvent(QMouseEvent *event)
{
    Component_lb *child = static_cast<Component_lb*>(childAt(event->pos()));
    if (!child)
        return;

    QPixmap pixmap = *child->pixmap();
    float value = (*child).getValue();
    int angle = (*child).getAngle();
    int type = (*child).getType();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos() - child->pos())<<value<<angle<<type;

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
        child->show();
        child->setPixmap(pixmap);
    }
}

