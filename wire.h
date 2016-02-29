#ifndef WIRE_H
#define WIRE_H
#include "component.h"

class Wire : public Component
{
public:
    Wire(int x, int y, int angle, int length,int node, float current=0.0);

    int getLength() const;
    void setLength(int value);


    int getNode() const;
    void setNode(int value);

private:
    int length,node;
    float current;
};

#endif // WIRE_H
