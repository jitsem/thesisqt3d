#include "source.h"
#include "component.h"
Source::Source(float v, int np, int nm, int x, int y, int angle)
    : nodep(np), nodem(nm)

{
        this->setValue(v);
    this->setXCoord(x);
    this->setYCoord(y);
    this->setAngle(angle);
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



