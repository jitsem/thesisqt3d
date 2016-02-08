#include "source.h"
#include "component.h"
Source::Source(float v, int np, int nm)
    : nodep(np), nodem(nm)

{
        Component::setValue(v);
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



