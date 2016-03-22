#ifndef WIRE_H
#define WIRE_H
#include "component.h"

//Class for wire
class Wire : public Component
{
public:
    Wire(float v, int x, int y, int ang, int length, int node, float current=0.0, int isG=0);

    //Getter/Setter for lenght
    int getLength() const;
    void setLength(int value);

    //Getter/Setter for wire node
    int getNode() const;
    void setNode(int value);

    //For compability, returns node
    int getNode1() const;
    int getNode2() const;

    //Getter/setter for isGoal(game-element)
    int getIsGoal() const;
    void setIsGoal(int value);

private:
    int length,node;
    int isGoal;
};

#endif // WIRE_H
