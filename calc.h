#ifndef CALC_H
#define CALC_H
#include <vector>
#include <memory>
#include <QFile>
#include "resistor.h"
#include "source.h"
#include "component.h"
#include <QDebug>
#include <QString>
#include "wire.h"

class Calc: public QObject
{
Q_OBJECT

public:
    Calc();


    //Methods invokable from QML
    Q_INVOKABLE void solveLevel();
    Q_INVOKABLE int getNumberOfSources(){return sources.size();}
    Q_INVOKABLE int getNumberOfResistors(){return resistors.size();}
    Q_INVOKABLE int getNumberOfWires(){return wires.size();}

    Q_INVOKABLE float voltageAtSource(int sourceNr){return sources.at(sourceNr)->getValue();}
    Q_INVOKABLE float resistanceAtResistor(int resNr){return resistors.at(resNr)->getValue();}

    Q_INVOKABLE int nodePAtSource(int sourceNr){return sources.at(sourceNr)->getNodep();}
    Q_INVOKABLE int nodeMAtSource(int sourceNr){return sources.at(sourceNr)->getNodem();}
    Q_INVOKABLE int node1AtResistor(int resNr){return resistors.at(resNr)->getNode1();}
    Q_INVOKABLE int node2AtResistor(int resNr){return resistors.at(resNr)->getNode2();}

    Q_INVOKABLE int numberOfNodes(){return sol.size();}
    Q_INVOKABLE float voltageAtNode(int nodeNr){return sol.at(nodeNr);}


    //Methodes voor tekenen van circuit, setters zijn tijdelijk. TODO verbeteren, teveel functies voor hetzelfde
    Q_INVOKABLE int getAngleOfResistor(int resNr){return resistors.at(resNr)->getAngle();}
    Q_INVOKABLE int getXCoordOfResistor(int resNr){return resistors.at(resNr)->getXCoord();}
    Q_INVOKABLE int getYCoordOfResistor(int resNr){return resistors.at(resNr)->getYCoord();}

    Q_INVOKABLE int getAngleOfSource(int soNr){return sources.at(soNr)->getAngle();}
    Q_INVOKABLE int getXCoordOfSource(int soNr){return sources.at(soNr)->getXCoord();}
    Q_INVOKABLE int getYCoordOfSource(int soNr){return sources.at(soNr)->getYCoord();}

    Q_INVOKABLE int getAngleOfWire(int wiNr){return wires.at(wiNr)->getAngle();}
    Q_INVOKABLE int getXCoordOfWire(int wiNr){return wires.at(wiNr)->getXCoord();}
    Q_INVOKABLE int getYCoordOfWire(int wiNr){return wires.at(wiNr)->getYCoord();}
    Q_INVOKABLE int getNodeOfWire(int wiNr){return wires.at(wiNr)->getNode();}
    Q_INVOKABLE int getLengthOfWire(int wiNr){return wires.at(wiNr)->getLength();}

    Q_INVOKABLE void readFile(QString s);

    //Methode voor juiste richtingen
    Q_INVOKABLE void correctAngles();

    std::vector<std::shared_ptr<Wire> > process_wire_line(QString& lijn);//TODO remove return type and push_back wires in global wires var
    void process_resistor_line(QString &lijn);
    void process_source_line(QString &lijn);

private:
    std::vector<float> computeNetwork(std::vector<std::shared_ptr<Component> > &s, std::vector<std::shared_ptr<Component> > &r, int nrOfNodes);
    //std::vector<std::shared_ptr<Component> > readFile(QFile *file);


    //variables for circuit

    std::vector<float> sol;
    std::vector<std::shared_ptr<Component>> sources;
    std::vector<std::shared_ptr<Component>> resistors;
    std::vector<std::shared_ptr<Wire>> wires;
};

#endif // CALC_H
