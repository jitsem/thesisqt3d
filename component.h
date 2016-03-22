#ifndef COMPONENT_H
#define COMPONENT_H

//Parent class for all components
class Component
{
public:
    Component(float val, int x, int y, int ang, int isAdj=0, float begValue=50, float sS=5.0, float cur = 0);


    //Getters/setter for electric parameters
    virtual float getCurrent() const;
    virtual void setCurrent(float value);

    virtual float getValue() const;
    virtual void setValue(float value);

    //Getters/setters for source nodes
    virtual int getNodep() const;
    virtual int getNodem() const;

    //Getters/setters for resistor nodes, seperate function due to slight differences
    virtual int getNode1() const;
    virtual int getNode2() const;

    //Getters/setters posistional parameters
    virtual int getAngle() const;
    virtual void setAngle(int value);

    virtual int getXCoord() const;
    virtual void setXCoord(int value);

    virtual int getYCoord() const;
    virtual void setYCoord(int value);

    //Getters/setters for game elements
    virtual int getIsAdjustable() const;
    virtual void setIsAdjustable(int value);

    virtual float getBeginValue() const;
    virtual void setBeginValue(float value);

    virtual float getStepSize() const;
    virtual void setStepSize(float value);

    //For non-wires, returns 1
    virtual int getLength();


protected:
    float value,current;
    int angle,xCoord,yCoord;

    // Define game elements of component
    int isAdjustable;
    float beginvalue;
    float stepSize;
};

#endif // COMPONENT_H
