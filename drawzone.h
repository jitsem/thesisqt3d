#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <memory>
#include <QFrame>


class Component_lb;
class QWidget;
class QMainWindow;

//Class for the right hand side of the application where the electric circuit is shown. inherits from QWidget
class DrawZone :public QFrame
{
    Q_OBJECT
public:
    DrawZone(QWidget *parent);
    
    //Rotates the child widget's pixmap to the angle set in it's properties
    void rotateToAngle(Component_lb &child);
    
    //Make sure node 1 and node 2 have correct x&y coordinates
    void updateNodePositions();
    
    //Function to snap components to nearest multiple of the grid
    int roundUp(int numToRound, int multiple);
    
    //Check if electric circuit is closed
    bool checkClosedCircuit();
    
    //Draw circuit from file
    void drawCircuit();
    
    //Get neighbouring components of current component that are present in list l ->key function in solving the node numbers
    QList<Component_lb *> getNeighbours(QList<Component_lb *> &l, Component_lb &current);
    
    //Solve 1 node and return components (!=wires) that are semi filled out (1 node number is correct)
    QList<Component_lb *> solveNode(QList<Component_lb *> &l, Component_lb *current, int &curnode, QList<Component_lb *> &stack);
    
    //Function that fills out node numbers in component_lb objects, uses getneighbours and solvenode intensively
    void calc_nodes();
    
    //Write current circuit to vectorlists for model of 3d representation
    void writeToVectors();
    
    //Add valuelabel to the component so that the user sees the value
    void addValueToComponent(Component_lb *&newIcon);
    
    //Getter and setter of global bool var groundpresent
    int getGroundpresent() const;
    void setGroundpresent(int value);
    
    //Set and remove gray background to pixmap when selecting, deselecting component
    Component_lb * removeGray(Component_lb & child);
    Component_lb * setGray(Component_lb & child);

public slots:
    
    //Slot for executing save to file method
    void slotTriggeredSave();
    
    //Ground selected wire with this slot
    void slotTriggeredGround();
    
protected:
    
    //Events, self explanatory
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    
    //Paintevent override to make sure the grid is drawn and the compnent connections (FUTURE, paasvakantie) are drawn
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;


private:

    int groundpresent=0;
    
    //VOORLOPIG The points that make up the connection between ground and wire for drawing
    QPoint polypoints[3];
};

#endif // DRAWZONE_H
