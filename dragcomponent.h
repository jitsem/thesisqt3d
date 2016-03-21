#ifndef DRAGCOMPONENT_H
#define DRAGCOMPONENT_H

#ifndef GRIDSIZE
#define GRIDSIZE 50
#endif

#include <QFrame>
#include <QWidget>

class DragComponent :public QFrame
{
    Q_OBJECT
public:
    DragComponent(QWidget *parent = 0);
protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

};

#endif // DRAGCOMPONENT_H


