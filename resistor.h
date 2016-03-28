#ifndef RESISTOR_H
#define RESISTOR_H

#include "component.h"

/**Class for resistor*/
class Resistor : public Component
{
public:
    /**Constructor for Resistor*/
    Resistor(float v, int n1, int n2, int x, int y, int angle, int isAdj=0, float begValue=50.0, float sS=5.0);
    /**Getter for Node 1*/
    int getNode1() const;
    /**Setter for Node 1*/
    void setNode1(int value);
    /**Getter for Node 2*/
    int getNode2() const;
    /**Setter for Node 2*/
    void setNode2(int value);


protected:
    /**Node 1 of Resistor*/
    int node1;
    /**Node 2of Resistor*/
    int node2;

};

#endif // RESISTOR_H
