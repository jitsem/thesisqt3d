#ifndef CALC_H
#define CALC_H
#include <vector>
#include <memory>
#include <QFile>
#include "resistor.h"
#include "source.h"
#include "component.h"
class Calc
{
public:
    Calc();

    std::vector<float> solveLevel(QFile *file);

private:
    std::vector<float> computeNetwork(std::vector<std::shared_ptr<Component> > s, std::vector<std::shared_ptr<Component> > r, int nrOfNodes);
    std::vector<std::shared_ptr<Component> > readFile(QFile *file);
};

#endif // CALC_H
