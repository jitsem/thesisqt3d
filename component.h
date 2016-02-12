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

protected:
    float value;
    int angle,xCoord,yCoord;
};

#endif // COMPONENT_H
