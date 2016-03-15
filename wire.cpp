#include "wire.h"

Wire::Wire(int x, int y, int ang, int length, int node, float current)
    :length(length),current(current),node(node)
{
    value=0;
    xCoord=x;
    angle=ang;
    yCoord=y;

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
