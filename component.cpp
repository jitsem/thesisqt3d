#include "component.h"

Component::Component()
{

}

int Component::getNodep() const
{

}

int Component::getNodem() const
{

}

int Component::getNode1() const
{

}

int Component::getNode2() const
{

}

int Component::getAngle() const
{
    return angle;
}

void Component::setAngle(int value)
{
    angle = value;
}

int Component::getXCoord() const
{
    return xCoord;
}

void Component::setXCoord(int value)
{
    xCoord = value;
}

int Component::getYCoord() const
{
    return yCoord;
}

void Component::setYCoord(int value)
{
    yCoord = value;
}


Component::Component(float v)
{
    this->value = v;
}

float Component::getValue() const
{
    return value;
}

void Component::setValue(float value)
{
    this->value=value;
}
