#include "component_lb.h"

#include <QDebug>
#include <QPoint>
#include <QMouseEvent>
#include <QWidget>

Component_lb::Component_lb(QWidget *parent, float val, int n1x, int n1y, int n2x, int n2y, int ang, int type, int selected, int n1, int n2, int adj, float beg, float ss, int go)
    :QLabel(parent),value(val), node1x(n1x),node1y(n1y),node2x(n2x),node2y(n2y),angle(ang),
      type(type),selected(selected),n1(n1),n2(n2),adjust(adj),begin(beg),stepSize(ss),goal(go)
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

float Component_lb::getValue() const
{
    return value;
}

void Component_lb::setValue(float val)
{
    value = val;
}

int Component_lb::getNode1x() const
{
    return node1x;
}

void Component_lb::setNode1x(int value)
{
    node1x = value;
}

int Component_lb::getNode1y() const
{
    return node1y;
}

void Component_lb::setNode1y(int value)
{
    node1y = value;
}

int Component_lb::getNode2x() const
{
    return node2x;
}

void Component_lb::setNode2x(int value)
{
    node2x = value;
}

int Component_lb::getNode2y() const
{
    return node2y;
}

void Component_lb::setNode2y(int value)
{
    node2y = value;
}

int Component_lb::getAngle() const
{
    return angle;
}

void Component_lb::setAngle(int value)
{
    angle = value;
}

int Component_lb::getType() const
{
    return type;
}

void Component_lb::setType(int value)
{
    type = value;
}


int Component_lb::getSelected() const
{
    return selected;
}

void Component_lb::setSelected(int value)
{
    selected = value;
}

int Component_lb::getN1() const
{
    return n1;
}

void Component_lb::setN1(int value)
{
    n1 = value;
}

int Component_lb::getN2() const
{
    return n2;
}

void Component_lb::setN2(int value)
{
    n2 = value;
}

int Component_lb::getAdjust() const
{
    return adjust;
}

void Component_lb::setAdjust(int value)
{
    adjust = value;
}

float Component_lb::getBegin() const
{
    return begin;
}

void Component_lb::setBegin(float value)
{
    begin = value;
}

float Component_lb::getStepSize() const
{
    return stepSize;
}

void Component_lb::setStepSize(float value)
{
    stepSize = value;
}

int Component_lb::getGoal() const
{
    return goal;
}

void Component_lb::setGoal(int value)
{
    goal = value;
}


