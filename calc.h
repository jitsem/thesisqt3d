#ifndef CALC_H
#define CALC_H
#include <vector>
#include <memory>
#include <QFile>
#include "resistor.h"
#include "source.h"
#include "component.h"

class Calc: public QObject
{
Q_OBJECT

public:
    Calc();


    //Methods invokable from QML
    Q_INVOKABLE void solveLevel(QString path);
    Q_INVOKABLE int getNumberOfSources(){return sources.size();}
    Q_INVOKABLE int getNumberOfResistors(){return resistors.size();}
    Q_INVOKABLE float voltageAtSource(int sourceNr){return sources.at(sourceNr)->getValue();}
    Q_INVOKABLE float resistanceAtResistor(int resNr){return resistors.at(resNr)->getValue();}
    Q_INVOKABLE int nodePAtSource(int sourceNr){return sources.at(sourceNr)->getNodep();}
    Q_INVOKABLE int nodeMAtSource(int sourceNr){return sources.at(sourceNr)->getNodem();}
    Q_INVOKABLE int node1AtResistor(int resNr){return resistors.at(resNr)->getNode1();}
    Q_INVOKABLE int node2AtResistor(int resNr){return resistors.at(resNr)->getNode2();}
    Q_INVOKABLE int numberOfNodes(){return sol.size();}
    Q_INVOKABLE float voltageAtNode(int nodeNr){return sol.at(nodeNr);}

private:
    std::vector<float> computeNetwork(std::vector<std::shared_ptr<Component> > &s, std::vector<std::shared_ptr<Component> > &r, int nrOfNodes);
    std::vector<std::shared_ptr<Component> > readFile(QFile *file);


    //variables for circuit

    std::vector<float> sol;
    std::vector<std::shared_ptr<Component>> sources;
    std::vector<std::shared_ptr<Component>> resistors;
};

#endif // CALC_H
