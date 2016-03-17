#ifndef SOURCE_H
#define SOURCE_H
#include "component.h"


class Source : public Component
{
public:
    Source(float v, int np, int nm, int x, int y, int angle, int isAdj=0, float begValue=50.0, float sS=5.0);
    int getNodep() const;
    void setNodep(int value);

    int getNodem() const;
    void setNodem(int value);

private:
    int nodep, nodem;
};

#endif // SOURCE_H
