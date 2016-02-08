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
