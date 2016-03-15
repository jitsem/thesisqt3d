#ifndef COMPONENT_H
#define COMPONENT_H


class Component
{
public:
    Component(float v);
    float getValue() const;
    void setValue(float value);
    Component();
    //virtual methods
    virtual int getNodep() const;
    virtual int getNodem() const;

    virtual int getNode1() const;
    virtual int getNode2() const;


    virtual int getAngle() const;
    virtual void setAngle(int value);

    virtual int getXCoord() const;
    virtual void setXCoord(int value);

    virtual int getYCoord() const;
    virtual void setYCoord(int value);

    virtual float getCurrent() const;
    virtual void setCurrent(float value);

    virtual bool getIsAdjustable() const;
    virtual void setIsAdjustable(bool value);

    virtual float getBeginvalue() const;
    virtual void setBeginvalue(float value);

    virtual float getStepSize() const;
    virtual void setStepSize(float value);

    //For non-wires, returns 1
    virtual int getLength();

protected:
    float value,current;
    int angle,xCoord,yCoord;

    // Compabiltiy with other team (game elements)
    bool isAdjustable;
    float beginvalue;
    float stepSize;
};

#endif // COMPONENT_H
