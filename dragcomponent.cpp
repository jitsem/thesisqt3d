#include "dragcomponent.h"
#include <QLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include <memory>
#include <QPixmap>
#include <QGraphicsItem>
#include <QDebug>
#include "component_lb.h"


DragComponent::DragComponent(QWidget *parent)
    :QFrame(parent)
{

    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(true);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->setAlignment(Qt::AlignTop);
    QWidgetList components;

    component_lb *sourceIcon = new component_lb(this,15,0,0,0,0,1,0);
    QPixmap source=QPixmap(":/assets/source_small.png");
    sourceIcon->setPixmap(source);
    sourceIcon->setScaledContents(true);
    //sourceIcon->setMaximumHeight(source.height());
    sourceIcon->setAttribute(Qt::WA_DeleteOnClose);
    sourceIcon->show();


    component_lb *res1 = new component_lb(this,100,3,4,20,4,1,1);
    QPixmap res2=QPixmap(":/assets/res_small.png");
    res1->setPixmap(res2);
    res1->move(0,10);
    res1->setScaledContents(true);
    res1->setAttribute(Qt::WA_DeleteOnClose);
    res1->show();

    component_lb *wire1 = new component_lb(this,0,3,4,20,4,1,2);
    QPixmap wir=QPixmap(":/assets/wire_small.png");
    wire1->setPixmap(wir);
    //wire1->move(0,10);
    wire1->setScaledContents(true);
    wire1->setAttribute(Qt::WA_DeleteOnClose);
    wire1->show();

    //foreach component
    //components.append(resistorIcon);
    components.append(sourceIcon);
    components.append(res1);
    components.append(wire1);
    mainLayout->addWidget(components[0]);
    mainLayout->addWidget(components[1]);
    mainLayout->addWidget(components[2]);


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
    component_lb *child = static_cast<component_lb*>(childAt(event->pos()));
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

