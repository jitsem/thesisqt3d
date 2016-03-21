#include "component_lb.h"
#include <QDebug>

component_lb::component_lb(QWidget *parent, float val, int n1x, int n1y, int n2x, int n2y, int ang, int type, int selected, int n1, int n2, int adj, float beg, float ss, int go)
    :QLabel(parent),value(val), node1x(n1x),node1y(n1y),node2x(n2x),node2y(n2y),angle(ang),
      type(type),selected(selected),n1(n1),n2(n2),adjust(adj),begin(beg),stepSize(ss),goal(go)
{


}


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


int component_lb::getSelected() const
{
    return selected;
}

void component_lb::setSelected(int value)
{
    selected = value;
}

int component_lb::getN1() const
{
    return n1;
}

void component_lb::setN1(int value)
{
    n1 = value;
}

int component_lb::getN2() const
{
    return n2;
}

void component_lb::setN2(int value)
{
    n2 = value;
}

int component_lb::getAdjust() const
{
    return adjust;
}

void component_lb::setAdjust(int value)
{
    adjust = value;
}

float component_lb::getBegin() const
{
    return begin;
}

void component_lb::setBegin(float value)
{
    begin = value;
}

float component_lb::getStepSize() const
{
    return stepSize;
}

void component_lb::setStepSize(float value)
{
    stepSize = value;
}

int component_lb::getGoal() const
{
    return goal;
}

void component_lb::setGoal(int value)
{
    goal = value;
}


