#ifndef COMPONENT_LB_H
#define COMPONENT_LB_H
#define COMPONENT_IS_GROUND 666

#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>

class component_lb : public QLabel
{
    Q_OBJECT
public:
    component_lb(QWidget * parent, float val, int n1x, int n1y, int n2x, int n2y, int ang, int type, qint64 nr=0, int selected=0,
                 int n1=-1, int n2=-1,int adj=0,float beg=50.0,float ss=5.0,int go = 0);

    float getValue() const;
    void setValue(float val);

    int getNode1x() const;
    void setNode1x(int value);

    int getNode1y() const;
    void setNode1y(int value);

    int getNode2x() const;
    void setNode2x(int value);

    int getNode2y() const;
    void setNode2y(int value);

    int getAngle() const;
    void setAngle(int value);

    int getType() const;
    void setType(int value);

    qint64 getNr() const;
    void setNr(qint64 value);



    int getSelected() const;
    void setSelected(int value);

    int getN1() const;
    void setN1(int value);

    int getN2() const;
    void setN2(int value);

    int getAdjust() const;
    void setAdjust(int value);

    float getBegin() const;
    void setBegin(float value);

    float getStepSize() const;
    void setStepSize(float value);

    int getGoal() const;
    void setGoal(int value);

private:
    float value;
    int node1x,node1y,node2x,node2y,angle,type,selected,n1,n2;

    //Variables for specifying parameters other team;
    int adjust,goal;
    float begin,stepSize;
    qint64 nr;
    //type 0 = source
    //1=resistor
    //2=wire
    //3=switch open
    //4=ground
    //TODO probably delete node2x and y because we've got 1 node and the angle
    //angle : 1 naar rechts
    //2: naar boven
    //3:naar links
    //4: naar onder
};

#endif // COMPONENT_LB_H
