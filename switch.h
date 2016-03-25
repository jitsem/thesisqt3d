#ifndef SWITCH_H
#define SWITCH_H

#include "resistor.h"

//Class for switch
class Switch : public Resistor
{
public:
    Switch(int np, int nm, int x, int y, int ang);

    //Toggles switch state
    void toggleSwitch();

    //Get switch state
    bool getUp() const;

private:
    bool up;
};

#endif // SWITCH_H
