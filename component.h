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
protected:
    float value;
};

#endif // COMPONENT_H
