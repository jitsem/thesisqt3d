#include "component.h"


Component::Component(float val, int x, int y, int ang, int isAdj, float begValue, float sS, float cur):
   value(val),xCoord(x),yCoord(y),angle(ang),isAdjustable(isAdj),beginvalue(begValue), stepSize(sS), current(cur)
{

}

int Component::getNodep() const
{
    return -1;
}

int Component::getNodem() const
{
    return -1;
}

int Component::getNode1() const
{
    return -1;
}

int Component::getNode2() const
{
    return -1;
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


float Component::getValue() const
{
    return value;
}

void Component::setValue(float value)
{
    this->value=value;
}

int Component::getIsAdjustable() const
{
    return isAdjustable;
}

void Component::setIsAdjustable(int value)
{
    isAdjustable = value;
}

float Component::getBeginValue() const
{
    return beginvalue;
}

void Component::setBeginValue(float value)
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

