#include "switch.h"
#include <limits>


Switch::Switch(int np, int nm, int x, int y, int ang):
    Resistor(1,np,nm,x,y,ang),up(false)
{

}

void Switch::toggleSwitch()
{
    if(up){
       value =1;
       up=false;
    }else{
        value = std::numeric_limits<float>::infinity();
        up=true;
    }

}

bool Switch::getUp() const
{
    return up;
}
