#ifndef DRAGCOMPONENT_H
#define DRAGCOMPONENT_H

#include <QFrame>


class QWidget;

/**Class for components in left dragzone*/
class DragComponent :public QFrame
{
    Q_OBJECT
public:
    /**Constructor for DragComponent*/
    DragComponent(QWidget *parent = 0);
protected:
    /**Catches dragEnterEvent*/
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    /**Catches dragMoveEvent*/
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    /**Catches dropEvent*/
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    /**Catches mousePressEvent*/
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

};

#endif // DRAGCOMPONENT_H


