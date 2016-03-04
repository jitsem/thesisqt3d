#include "component_lb.h"
#include <QDebug>
component_lb::component_lb(QWidget *parent, float val, int n1x, int n1y, int n2x, int n2y, int ang, int type, qint64 nr, int selected)
    :QLabel(parent),value(val), node1x(n1x),node1y(n1y),node2x(n2x),node2y(n2y),angle(ang),type(type),nr(nr),selected(selected)
{
    //connect (this,SIGNAL(clicked(const QPoint & )),this,SLOT(slotClicked()));


}
//toremove
//void component::slotClicked()
//{
//    //qDebug()<<"Clicked";
//}

//void component::mouseReleaseEvent(QMouseEvent *event)
//{
    //qDebug()<<"mouse release event van component";
    //emit clicked((event->pos()));
    //TODO delete, done in drawzone
//}

float component_lb::getValue() const
{
    return value;
}

void component_lb::setValue(float val)
{
    value = val;
}

int component_lb::getNode1x() const
{
    return node1x;
}

void component_lb::setNode1x(int value)
{
    node1x = value;
}

int component_lb::getNode1y() const
{
    return node1y;
}

void component_lb::setNode1y(int value)
{
    node1y = value;
}

int component_lb::getNode2x() const
{
    return node2x;
}

void component_lb::setNode2x(int value)
{
    node2x = value;
}

int component_lb::getNode2y() const
{
    return node2y;
}

void component_lb::setNode2y(int value)
{
    node2y = value;
}

int component_lb::getAngle() const
{
    return angle;
}

void component_lb::setAngle(int value)
{
    angle = value;
}

int component_lb::getType() const
{
    return type;
}

void component_lb::setType(int value)
{
    type = value;
}

qint64 component_lb::getNr() const
{
    return nr;
}

void component_lb::setNr(qint64 value)
{
    nr = value;
}

int component_lb::getSelected() const
{
    return selected;
}

void component_lb::setSelected(int value)
{
    selected = value;
}
