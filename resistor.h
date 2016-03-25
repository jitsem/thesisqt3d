#ifndef RESISTOR_H
#define RESISTOR_H

#include "component.h"

//Class for resistor
class Resistor : public Component
{
public:
    Resistor(float v, int n1, int n2, int x, int y, int angle, int isAdj=0, float begValue=50.0, float sS=5.0);

    //Getters/Setters for Nodes
    int getNode1() const;
    void setNode1(int value);

    int getNode2() const;
    void setNode2(int value);


protected:
    int node1,node2;

};

#endif // RESISTOR_H
