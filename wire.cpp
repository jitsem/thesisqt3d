#include "wire.h"

Wire::Wire(float v, int x, int y, int ang, int length, int node, float current, int isVG, int isCG)
    :Component(v,x,y,ang,0,0,0,current),length(length),node(node),isVoltageGoal(isVG),isCurrentGoal(isCG)
{


}


int Wire::getLength() const
{
    return length;
}

void Wire::setLength(int value)
{
    length = value;
}

int Wire::getNode() const
{
    return node;
}

void Wire::setNode(int value)
{
    node = value;
}

int Wire::getNode1() const
{
    return getNode();
}

int Wire::getNode2() const
{
    return getNode();
}

int Wire::getIsVoltageGoal() const
{
    return isVoltageGoal;
}

void Wire::setIsVoltageGoal(int value)
{
    isVoltageGoal = value;
}

int Wire::getIsCurrentGoal() const
{
    return isCurrentGoal;
}

void Wire::setIsCurrentGoal(int value)
{
    isCurrentGoal = value;
}
