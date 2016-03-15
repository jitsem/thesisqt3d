#include "component.h"

//TODO Overerving deftiger maken

Component::Component():
   isAdjustable(true),beginvalue(value), stepSize(5.0)
{

}

int Component::getNodep() const
{
    return 0;
}

int Component::getNodem() const
{
    return 0;
}

int Component::getNode1() const
{
    return 0;
}

int Component::getNode2() const
{
    return 0;
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

float Component::getCurrent() const
{
    return current;
}

void Component::setCurrent(float value)
{
    current = value;
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

//Functions for other teams game elements
bool Component::getIsAdjustable() const
{
    return isAdjustable;
}

void Component::setIsAdjustable(bool value)
{
    isAdjustable = value;
}

float Component::getBeginvalue() const
{
    return beginvalue;
}

void Component::setBeginvalue(float value)
{
    beginvalue = value;
}

float Component::getStepSize() const
{
    return stepSize;
}

void Component::setStepSize(float value)
{
    stepSize = value;
}

int Component::getLength()
{
    return 1;
}

