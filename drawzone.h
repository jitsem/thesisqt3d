#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <memory>
#include <QFrame>


class Component_lb;
class QWidget;
class QMainWindow;

/**Class for the right hand side of the application where the electric circuit is shown. inherits from QFrame*/
class DrawZone :public QFrame
{
    Q_OBJECT
public:
    /**Constructor van Drawzone*/
    DrawZone(QWidget *parent);
    /**Rotates the child widget's pixmap to the angle set in it's properties*/
    void rotateToAngle(Component_lb &child);
    /**Make sure node 1 and node 2 have correct x&y coordinates*/
    void updateNodePositions();
    /**Function to snap components to nearest multiple of the grid*/
    int roundUp(int numToRound, int multiple);
    /**Check if electric circuit is closed*/
    bool checkClosedCircuit();
    /**Draw circuit from file*/
    void drawCircuit();
    /**Get neighbouring components of current component that are present in list l ->key function in solving the node numbers*/
    QList<Component_lb *> getNeighbours(QList<Component_lb *> &l, Component_lb &current);
    /**Solve 1 node and return components (!=wires) that are semi filled out (1 node number is correct)*/
    QList<Component_lb *> solveNode(QList<Component_lb *> &l, Component_lb *current, int &curnode, QList<Component_lb *> &stack);
    /**Function that fills out node numbers in component_lb objects, uses getNeighbours() and solvenode() intensively*/
    void calc_nodes();
    /**Write current circuit to vectorlists for model of 3d representation*/
    void writeToVectors();
    /**Add valuelabel to the component so that the user sees the value*/
    void addValueToComponent(Component_lb *&newIcon);
    /**Getter of global bool var groundpresent*/
    int getGroundpresent() const;
    /**Setter of global bool var groundpresent*/
    void setGroundpresent(int value);
    /**Remove gray background to pixmap when deselecting component*/
    Component_lb * removeGray(Component_lb & child);
    /**Set gray background to pixmap when selecting component*/
    Component_lb * setGray(Component_lb & child);
    /**Connect components on the grid */
    void connectComponents();
    /**Draws a wire element of size 1 at point p with direction dir*/
    void addWire(QPoint &p, int dir);
    /**Adjust ScreenSize to Components*/
    void adjustScreenSize();

public slots:

    /**Slot for executing save to file method*/
    void slotTriggeredSave();
    /**Round selected wire with this slot*/
    void slotTriggeredGround();
    /**capture change of connect tool*/
    void slotConnectChanged(bool b);


protected:

    /**Catches doubleClickEvent*/
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    /**Catches dragEnterEvent*/
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    /**Catches dragMoveEvent*/
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    /**Catches dropEvent*/
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    /**Catches dmousePressEvent*/
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    /**Catches mouseReleaseEvent*/
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    /**Catches doubleClickEvent*/
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    /**Override to make sure the grid is drawn and the component connections (In the Future) are drawn*/
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    /**Variable to check for Ground*/
    int groundpresent=0;
    /**The points that make up the connection between ground and wire for drawing -- TEMPORARY*/
    QPoint polypoints[3];
    /**Needed to determine move or click*/
    QPoint  dragStartPosition;
    /**Tool nr 1 = connect two components */
    int doubleClicked=0,selectedTool=0;
    /**The points that make up the connection between components for drawing */
    QVector<QPoint> connectPoints;
    /**Vector containing the points that connect components */
      QVector<QPoint> nodes;
    /**Position of red dot that shows on hover while connecting components */
      QPoint redDotPos;


};


#endif // DRAWZONE_H
