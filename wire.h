#ifndef WIRE_H
#define WIRE_H
#include "component.h"

class Wire : public Component
{
public:
    Wire(float v, int x, int y, int ang, int length, int node, float current=0.0, int isG=0);

    int getLength() const;
    void setLength(int value);


    int getNode() const;
    void setNode(int value);

    //For compability
    int getNode1() const;
    int getNode2() const;


    int getIsGoal() const;
    void setIsGoal(int value);

private:
    int length,node;
    float current;
    int isGoal;
};

#endif // WIRE_H
