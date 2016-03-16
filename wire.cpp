#include "wire.h"

Wire::Wire(float v, int x, int y, int ang, int length, int node, float current, int isG)
    :Component(v,x,y,ang),length(length),current(current),node(node),isGoal(isG)
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

int Wire::getIsGoal() const
{
    return isGoal;
}

void Wire::setIsGoal(int value)
{
    isGoal = value;
}
