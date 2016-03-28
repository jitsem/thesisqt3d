#ifndef SOURCE_H
#define SOURCE_H

#include "component.h"

/**Class for source*/
class Source : public Component
{
public:
    /**Constructor for Source*/
    Source(float v, int np, int nm, int x, int y, int angle, int isAdj=0, float begValue=50.0, float sS=5.0);
    /**Getter for Node positive*/
    int getNodep() const;
    /**Setter for Node positive*/
    void setNodep(int value);
    /**Getter for Node negative*/
    int getNodem() const;
    /**Setter for Node negative*/
    void setNodem(int value);

private:
    /**Node positive of Source*/
    int nodep;
    /**Node Negative of Source*/
    int nodem;
}; 

#endif // SOURCE_H
