#ifndef DRAWZONE_H
#define DRAWZONE_H


#include <QWidget>
#include <QFrame>
#include <memory>

#include "component_lb.h"
#include "component.h"


class DrawZone :public QFrame
{
    Q_OBJECT
public:
    DrawZone(QWidget *parent);
    void rotateToAngle(component_lb &child);
    void updateNodePositions();
    int roundUp(int numToRound, int multiple);
    bool checkClosedCircuit();
    void drawCircuit();

    QList<component_lb *> getNeighbours(QList<component_lb *> &l, component_lb &current);
    QList<component_lb *> solveNode(QList<component_lb *> &l, component_lb *current, int &curnode, QList<component_lb *> &stack);
    void calc_nodes();
    void writeToVectors();

    void addValueToComponent(component_lb *&newIcon);

public slots:
    void slotTriggeredRotate();
    void slotTriggeredDelete();
    void slotTriggeredSave();
    void slotTriggeredConnect();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    component_lb * removeGray(component_lb & child);
    component_lb * setGray(component_lb & child);
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;


private:
    int connect=0;
    QPoint start,stop;
    int width;
    int height;
};

#endif // DRAWZONE_H
