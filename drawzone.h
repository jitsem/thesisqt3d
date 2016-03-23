#ifndef DRAWZONE_H
#define DRAWZONE_H

#ifndef GRIDSIZE
#define GRIDSIZE 50
#endif

#include <QWidget>
#include <QFrame>
#include <memory>
#include <QMainWindow>

#include "component_lb.h"
#include "component.h"
#include "mainwindow.h"

//Class for the right hand side of the application where the electric circuit is shown. inherits from QWidget
class DrawZone :public QFrame
{
    Q_OBJECT
public:
    DrawZone(QWidget *parent);
    //rotates the child widget's pixmap to the angle set in it's properties
    void rotateToAngle(component_lb &child);
    //make sure node 1 and node 2 have correct x&y coordinates
    void updateNodePositions();
    //function to snap components to nearest multiple of the grid
    int roundUp(int numToRound, int multiple);
    //check if electric circuit is closed
    bool checkClosedCircuit();
    //draw circuit from file
    void drawCircuit();
    //get neighbouring components of current component that are present in list l ->key function in solving the node numbers
    QList<component_lb *> getNeighbours(QList<component_lb *> &l, component_lb &current);
    //solve 1 node and return components (!=wires) that are semi filled out (1 node number is correct)
    QList<component_lb *> solveNode(QList<component_lb *> &l, component_lb *current, int &curnode, QList<component_lb *> &stack);
    //function that fills out node numbers in component_lb objects, uses getneighbours and solvenode intensively
    void calc_nodes();
    //write current circuit to vectorlists for model of 3d representation
    void writeToVectors();
    //add valuelabel to the component so that the user sees the value
    void addValueToComponent(component_lb *&newIcon);
    //getter and setter of global bool var groundpresent
    int getGroundpresent() const;
    void setGroundpresent(int value);
    //set and remove gray background to pixmap when selecting, deselecting component
    component_lb * removeGray(component_lb & child);
    component_lb * setGray(component_lb & child);

public slots:
    //slot for executing save to file method
    void slotTriggeredSave();
    //ground selected wire with this slot
    void slotTriggeredGround();
    //events, self explanatory
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    //paintevent override to make sure the grid is drawn and the compnent connections (FUTURE, paasvakantie) are drawn
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;


private:

    int groundpresent=0;
    // pointer to mainwindow, needed to change active state of buttons
    MainWindow *main_Window;
    //VOORLOPIG The points that make up the connection between ground and wire for drawing
    QPoint polypoints[3];
};

#endif // DRAWZONE_H
