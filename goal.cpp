#include "goal.h"

Goal::Goal(int x, int y, int node):
    goalNode(node),goalCoords(x,y)
{

}

int Goal::getGoalNode() const
{
    return goalNode;
}

void Goal::setGoalNode(int value)
{
    goalNode = value;
}

QPoint Goal::getGoalCoords() const
{
    return goalCoords;
}

void Goal::setGoalCoords(const QPoint &value)
{
    goalCoords = value;
}
