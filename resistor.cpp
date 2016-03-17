#include "resistor.h"

Resistor::Resistor(float v, int n1, int n2, int x, int y, int angle, int isAdj,float begValue,float sS)
 : Component(v,x,y,angle,isAdj,begValue, sS), node1(n1),node2(n2)
{

}

int Resistor::getNode1() const
{
    return node1;
}

void Resistor::setNode1(int value)
{
    node1 = value;
}

int Resistor::getNode2() const
{
    return node2;
}

void Resistor::setNode2(int value)
{
    node2 = value;
}



