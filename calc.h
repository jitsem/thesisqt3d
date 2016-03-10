#ifndef CALC_H
#define CALC_H
#include <vector>
#include <memory>
#include <QFile>
#include <QDebug>
#include <QString>
#include "resistor.h"
#include "source.h"
#include "component.h"
#include "wire.h"
#include "switch.h"



class Calc: public QObject
{
Q_OBJECT

public:

    //Returns instance of this class
    static std::shared_ptr<Calc> Instance();

    //Methods invokable from QML,voor tekenen van circuit. TODO verbeteren, teveel functies voor hetzelfde
    Q_INVOKABLE void solveLevel();

    //Nodes
    Q_INVOKABLE int numberOfNodes(){return sol.size();}
    Q_INVOKABLE float voltageAtNode(int nodeNr){return sol.at(nodeNr);}

    //Resistors
    Q_INVOKABLE int getNumberOfResistors(){return resistors.size();}
    Q_INVOKABLE float resistanceAtResistor(int resNr){return resistors.at(resNr)->getValue();}
    Q_INVOKABLE void increaseResistanceAtResistor(int resNr){resistors.at(resNr)->setValue(resistors.at(resNr)->getValue()+25);}
    Q_INVOKABLE void decreaseResistanceAtResistor(int resNr){if(resistors.at(resNr)->getValue()>25){resistors.at(resNr)->setValue(resistors.at(resNr)->getValue()-25);}}
    Q_INVOKABLE float getCurrentofResistor(int resNr){return resistors.at(resNr)->getCurrent();}
    Q_INVOKABLE int getAngleOfResistor(int resNr){return resistors.at(resNr)->getAngle();}
    Q_INVOKABLE int getXCoordOfResistor(int resNr){return resistors.at(resNr)->getXCoord();}
    Q_INVOKABLE int getYCoordOfResistor(int resNr){return resistors.at(resNr)->getYCoord();}
    Q_INVOKABLE int node1AtResistor(int resNr){return resistors.at(resNr)->getNode1();}
    Q_INVOKABLE int node2AtResistor(int resNr){return resistors.at(resNr)->getNode2();}
    bool addResistor(std::shared_ptr<Resistor> r);

    //Sources
    Q_INVOKABLE int getNumberOfSources(){return sources.size();}
    Q_INVOKABLE float getVoltageAtSource(int sourceNr){return sources.at(sourceNr)->getValue();}
    Q_INVOKABLE void increaseVoltageAtSource(int sourceNr){sources.at(sourceNr)->setValue(sources.at(sourceNr)->getValue()+5);}
    Q_INVOKABLE void decreaseVoltageAtSource(int sourceNr){if(sources.at(sourceNr)->getValue()>5){sources.at(sourceNr)->setValue(sources.at(sourceNr)->getValue()-5);}}
    Q_INVOKABLE float getCurrentofSource(int sourceNr){return sources.at(sourceNr)->getCurrent();}
    Q_INVOKABLE int getAngleOfSource(int soNr){return sources.at(soNr)->getAngle();}
    Q_INVOKABLE int getXCoordOfSource(int soNr){return sources.at(soNr)->getXCoord();}
    Q_INVOKABLE int getYCoordOfSource(int soNr){return sources.at(soNr)->getYCoord();}
    Q_INVOKABLE int nodePAtSource(int sourceNr){return sources.at(sourceNr)->getNodep();}
    Q_INVOKABLE int nodeMAtSource(int sourceNr){return sources.at(sourceNr)->getNodem();}
        bool addSource(std::shared_ptr<Source> s);

    //Wires
    Q_INVOKABLE int getNumberOfWires(){return wires.size();}
    Q_INVOKABLE float getCurrentofWire(int wiNr){return wires.at(wiNr)->getCurrent();}
    Q_INVOKABLE int getAngleOfWire(int wiNr){return wires.at(wiNr)->getAngle();}
    Q_INVOKABLE int getXCoordOfWire(int wiNr){return wires.at(wiNr)->getXCoord();}
    Q_INVOKABLE int getYCoordOfWire(int wiNr){return wires.at(wiNr)->getYCoord();}
    Q_INVOKABLE int getNodeOfWire(int wiNr){return wires.at(wiNr)->getNode();}
    Q_INVOKABLE int getLengthOfWire(int wiNr){return wires.at(wiNr)->getLength();}
        bool addWire(std::shared_ptr<Wire> w);

    //Switches
    Q_INVOKABLE int getNumberOfSwitches(){return switches.size();}
    Q_INVOKABLE bool isSwitchUp(int swNr){return (switches.at(swNr)->getUp());}
    Q_INVOKABLE int getAngleOfSwitch(int swNr){return switches.at(swNr)->getAngle();}
    Q_INVOKABLE int getXCoordOfSwitch(int swNr){return switches.at(swNr)->getXCoord();}
    Q_INVOKABLE int getYCoordOfSwitch(int swNr){return switches.at(swNr)->getYCoord();}
    Q_INVOKABLE int node1AtSwitch(int swNr){return switches.at(swNr)->getNode1();}
    Q_INVOKABLE int node2AtSwitch(int swNr){return switches.at(swNr)->getNode2();}
    Q_INVOKABLE void toggleSwitch(int swNr){switches.at(swNr)->toggleSwitch();}

            bool addSwitch(std::shared_ptr<Switch> s);

            //Read in a new file
    Q_INVOKABLE void readFile();

    //Methode voor juiste richtingen en stromen
    void correctAngles();
    void setCurrentsOfResistorsAndSwitches();
    void setCurrentsOfWires();
    void setCurrentsOfSwitchedWires();
    void setCurrentsOfStrayWires();

    //Methodes for reading files
    void process_wire_line(QString& lijn);
    void process_resistor_line(QString &lijn);
    void process_source_line(QString &lijn);
    void process_switch_line(QString &lijn);

    //Methodes for writing back file
    Q_INVOKABLE void writeBackToFile();

    void setFileName(const QString &value);
    QString getFileName() const;

    void emptyVectors();
    //Getters and settters for vectors
    std::vector<std::shared_ptr<Wire>> getWires() const;
    std::vector<std::shared_ptr<Component> > getResistors() const;
    std::vector<std::shared_ptr<Component> > getSources() const;
    std::vector<std::shared_ptr<Switch> > getSwitches() const;

private:

    Calc();
    static std::shared_ptr<Calc> instance;

    std::vector<float> computeNetwork(int nrOfNodes);


    //variables for circuit

    std::vector<float> sol;
    std::vector<std::shared_ptr<Component>> sources;
    std::vector<std::shared_ptr<Component>> resistors;
    std::vector<std::shared_ptr<Wire>> wires;
    std::vector<std::shared_ptr<Switch>> switches;
    QString fileName;
};

#endif // CALC_H
