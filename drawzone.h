#ifndef DRAWZONE_H
#define DRAWZONE_H


#include <QWidget>
#include <QFrame>
#include <memory>

#include "component_lb.h"
#include "component.h"
#include "wire.h"

class DrawZone :public QFrame
{
    Q_OBJECT
public:
    DrawZone(QWidget *parent);
    void rotateToAngle(component_lb &child);
    void updateNodePositions();
    int roundUp(int numToRound, int multiple);

public slots:
    void slotTriggeredRotate();
    void slotTriggeredDelete();
    void slotTriggeredSave();
    void slotTriggered3D_Preview();
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

};

#endif // DRAWZONE_H
