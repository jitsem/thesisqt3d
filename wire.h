#ifndef WIRE_H
#define WIRE_H

#include "component.h"

/**Class for wire*/
class Wire : public Component
{
public:
    /**Contructor for Wire*/
    Wire(float v, int x, int y, int ang, int length, int node, float current=0.0, int isG=0);
    /**Getter for lenght*/
    int getLength() const;
    /**Setter for lenght*/
    void setLength(int value);
    /**Getter for wire node*/
    int getNode() const;
    /**Setter for wire node*/
    void setNode(int value);
    /**For compability, returns node*/
    int getNode1() const;
    /**For compability, returns node*/
    int getNode2() const;
    /**Getter for isGoal*/
    int getIsGoal() const;
    /**Setter for isGoal*/
    void setIsGoal(int value);

private:
    /**Lenght of Wire*/
    int length;
    /**Node of Wire*/
    int node;
    /**Goal flag for Wire, for usage in other teams' game*/
    int isGoal;
};

#endif // WIRE_H
