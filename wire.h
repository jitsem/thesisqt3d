#ifndef WIRE_H
#define WIRE_H

#include "component.h"

/**Class for wire*/
class Wire : public Component
{
public:
    /**Contructor for Wire*/
    Wire(float v, int x, int y, int ang, int length, int node, float current=0.0, int isVG=0, int isCG=0);
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
    /**Getter for isVoltageGoal*/
    int getIsVoltageGoal() const;
    /**Setter for isVoltageGoal*/
    void setIsVoltageGoal(int value);
    /**Getter for isCurrentGoal*/
    int getIsCurrentGoal() const;
    /**Setter for isCurrentGoal*/
    void setIsCurrentGoal(int value);

private:
    /**Length of Wire*/
    int length;
    /**Node of Wire*/
    int node;
    /**Goal Voltage flag for Wire, for usage in other teams' game*/
    int isVoltageGoal;
    /**Goal Voltage flag for Wire, for usage in other teams' game*/
    int isCurrentGoal;
};

#endif // WIRE_H
