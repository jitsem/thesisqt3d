#ifndef COMPONENT_LB_H
#define COMPONENT_LB_H
#define COMPONENT_IS_GROUND 666

#include <QLabel>


class QWidget;
/**Inherits from QWidget and is class that holds all the component objects in the levenbuilder*/
class Component_lb : public QLabel
{
    Q_OBJECT
public:
    /**Constructor for Component_lb class*/
    Component_lb(QWidget * parent, float val, int n1x, int n1y, int n2x, int n2y, int ang, int type, int selected=0,
                 int n1=-1, int n2=-1,int adj=0,float beg=50.0,float ss=5.0,int gov = 0,int goc=0);
    /**Getter for value*/
    float getValue() const;
    /**Setter for value*/
    void setValue(float val);
    /**Getter for Node1 x position*/
    int getNode1x() const;
    /**Setter for Node1 x position*/
    void setNode1x(int value);
    /**Getter for Node1 y position*/
    int getNode1y() const;
    /**Setter for Node1 y position*/
    void setNode1y(int value);
    /**Getter for Node2 x position*/
    int getNode2x() const;
    /**Setter for Node2 x position*/
    void setNode2x(int value);
    /**Getter for Node2 y position*/
    int getNode2y() const;
    /**Setter for Node2 y position*/
    void setNode2y(int value);
    /**Getter for Angle of component*/
    int getAngle() const;
    /**Setter for Angle of component*/
    void setAngle(int value);
    /**Getter for Type of component*/
    int getType() const;
    /**Setter for Type of component*/
    void setType(int value);
    /**Getter for selected state of component*/
    int getSelected() const;
    /**Setter for selected state of component*/
    void setSelected(int value);
    /**Getter for Node1 */
    int getN1() const;
    /**Setter for Node1 */
    void setN1(int value);
    /**Getter for Node2 */
    int getN2() const;
    /**Setter for Node2 */
    void setN2(int value);
    /**Getter for Adjustable boolean var */
    int getAdjust() const;
    /**Setter for Adjustable boolean var */
    void setAdjust(int value);
    /**Getter for begin value of component */
    float getBegin() const;
    /**Setter for begin value of component */
    void setBegin(float value);
    /**Getter for stepSize of component */
    float getStepSize() const;
    /**Setter for stepSize of component */
    void setStepSize(float value);
    /**Getter for voltage goal boolean var */
    int getVoltageGoal() const;
    /**Setter for voltage goal boolean var */
    void setVoltageGoal(int value);
    /**Getter for current goal boolean var */
    int getCurrentGoal() const;
    /**Setter for current goal boolean var */
    void setCurrentGoal(int value);

private:
    /**Value of Component*/
    float value;
    /**XCoord of 1ste node*/
    int node1x;
    /**YCoord of 1ste node*/
    int node1y;
    /**XCoord of 2nd node*/
    int node2x;
    /**YCoord of 2nd node*/
    int node2y;
    /**Flag to show if component is selected*/
    int selected;
    /**Node nr 1*/
    int n1;
    /**Node nr 2*/
    int n2;
    /**Type info
     * 0=source
     * 1=resistor
     * 2=wire
     * 3=switch
     * 4=ground
     * */
    int type;
    /**Angle of component
     * 1=to the right
     * 2=up
     * 3=to the left
     * 4=down
     * */
    int angle;

    /**Flag to show if component is adjustable, game element of other teams' app*/
    int adjust;
    /**Flag to show if component is a voltage goal (Wire only), game element of other teams' app*/
    int voltageGoal;
     /**Flag to show if component is a current goal (Wire only), game element of other teams' app*/
    int currentGoal;
    /**BeginValue of component, game element of other teams' app*/
    float begin;
    /**StepSize of component, game element of other teams' app*/
    float stepSize;

};

#endif // COMPONENT_LB_H
