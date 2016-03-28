#ifndef COMPONENT_H
#define COMPONENT_H

/**Parent class for all components*/
class Component
{
public:
    /**Contructor for Component*/
    Component(float val, int x, int y, int ang, int isAdj=0, float begValue=50, float sS=5.0, float cur = 0);
    /**Getter for current*/
    virtual float getCurrent() const;
    /**Setter for current*/
    virtual void setCurrent(float value);
    /**Getter for value*/
    virtual float getValue() const;
    /**Setter for value*/
    virtual void setValue(float value);
    /**Getters for positive Source node*/
    virtual int getNodep() const;
    /**Getter for negative source node*/
    virtual int getNodem() const;
    /**Getter for Resistor node 1, seperate function due to slight differences*/
    virtual int getNode1() const;
    /**Getter for Resistor node 1, seperate function due to slight differences*/
    virtual int getNode2() const;
    /**Getter for angle*/
    virtual int getAngle() const;
    /**Setter for angle*/
    virtual void setAngle(int value);
    /**Getter for Xcoord*/
    virtual int getXCoord() const;
    /**Setter for XCoord*/
    virtual void setXCoord(int value);
    /**Getter for Ycoord*/
    virtual int getYCoord() const;
    /**Setter for YCoord*/
    virtual void setYCoord(int value);
    /**Getter for isAdjustable*/
    virtual int getIsAdjustable() const;
    /**Setter for isAdjustable*/
    virtual void setIsAdjustable(int value);
    /**Getter for beginValue*/
    virtual float getBeginValue() const;
    /**Setter for beginValue*/
    virtual void setBeginValue(float value);
    /**Getter for stepSize*/
    virtual float getStepSize() const;
    /**Setter for stepSize*/
    virtual void setStepSize(float value);
    /**For non-wires, returns 1. Should be inherited by Wire*/
    virtual int getLength();


protected:
    /**Value of Component*/
    float value;
    /**Current trough Component*/
    float current;
    /**Angle of Component*/
    int angle;
    /**X-Coordinate of Component*/
    int xCoord;
    /**Y-Coordinate of Component*/
    int yCoord;
    /**Defines if a Component is adjustable. Used in other teams' game*/
    int isAdjustable;
    /**BeginValue of Component. Used in other teams' game*/
    float beginvalue;
    /**StepSize of Component. Used in other teams' game*/
    float stepSize;
};

#endif // COMPONENT_H
