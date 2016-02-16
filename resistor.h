#ifndef RESISTOR_H
#define RESISTOR_H
#include "component.h"


class Resistor : public Component
{
public:
    Resistor(float v,int n1,int n2,int x,int y,int angle);
    int getNode1() const;
    void setNode1(int value);

    int getNode2() const;
    void setNode2(int value);


private:
    int node1,node2;

};

#endif // RESISTOR_H
