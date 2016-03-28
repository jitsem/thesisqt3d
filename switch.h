#ifndef SWITCH_H
#define SWITCH_H

#include "resistor.h"

/**Class for switch*/
class Switch : public Resistor
{
public:
    /**Contructor for Switch*/
    Switch(int np, int nm, int x, int y, int ang);
    /**Toggles switch state*/
    void toggleSwitch();
    /**Getter for switch state*/
    bool getUp() const;

private:
    /**Switch state*/
    bool up;
};

#endif // SWITCH_H
