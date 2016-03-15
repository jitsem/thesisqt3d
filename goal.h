#ifndef GOAL_H
#define GOAL_H
#include <QPoint>

//Class to define a goal: For compability with other team
class Goal
{
public:
    Goal(int x, int y, int node);
    int getGoalNode() const;
    void setGoalNode(int value);

    QPoint getGoalCoords() const;
    void setGoalCoords(const QPoint &value);

private:
    int goalNode;
    QPoint goalCoords;
};

#endif // GOAL_H
