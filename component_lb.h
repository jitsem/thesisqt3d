#ifndef COMPONENT_LB_H
#define COMPONENT_LB_H

#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>

class component_lb : public QLabel
{
    Q_OBJECT
public:
    component_lb(QWidget * parent, float val, int n1x, int n1y, int n2x, int n2y, int ang, int type, qint64 nr=0, int selected=0);

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

//signals:
//    void clicked(const QPoint & pos);

//public slots:
//    void slotTriggeredRotate();

//protected:
//   void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;

    int getSelected() const;
    void setSelected(int value);

private:
    float value;
    int node1x,node1y,node2x,node2y,angle,type,selected;
    qint64 nr;
    //type 0 = source
    //1=resistor
    //2=wire
    //probably delete node2x and y because we've got 1 node and the angle
    //angle : 1 naar rechts
    //2: naar boven
    //3:naar links
    //4: naar onder
};

#endif // COMPONENT_LB_H
