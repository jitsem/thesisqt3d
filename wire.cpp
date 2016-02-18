#include "wire.h"

Wire::Wire(int x, int y, int angle, int length, int node, float current)
    :length(length),current(current),node(node)
{
    this->setValue(0);
    this->setXCoord(x);
    this->setAngle(angle);
    this->setYCoord(y);
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
