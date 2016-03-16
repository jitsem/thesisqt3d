#include "source.h"

Source::Source(float v, int np, int nm, int x, int y, int angle,int isAdj,float begValue,float sS)
    : Component(v,x,y,angle,isAdj,begValue,sS),nodep(np), nodem(nm)

{

}



int Source::getNodep() const
{
    return nodep;
}

void Source::setNodep(int value)
{
    nodep = value;
}

int Source::getNodem() const
{
    return nodem;
}

void Source::setNodem(int value)
{
    nodem = value;
}



