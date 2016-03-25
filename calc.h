#ifndef CALC_H
#define CALC_H

#include <vector>
#include <memory>
#include <QObject>

#include "resistor.h"
#include "source.h"
#include "wire.h"
#include "switch.h"

class Qstring;


//Class where all major calculations and I/O happen
class Calc: public QObject
{
Q_OBJECT

public:

    /**Make sure there is only one instance of the Calc*/
    static std::shared_ptr<Calc> Instance();

    /**Main methode for solving a level, invokable from QML*/
    Q_INVOKABLE bool solveLevel();

    /**Get number of Nodes in circuit, invokable from QML*/
    Q_INVOKABLE int numberOfNodes(){return sol.size();}

    /**Get voltage at a certain Node*/
    Q_INVOKABLE float voltageAtNode(int nodeNr){return sol.at(nodeNr);}

    /**Get number of Resistors, invokable from QML*/
    Q_INVOKABLE int getNumberOfResistors(){return resistors.size();}
    /**Get resistance at Resistor at resNr, invokable from QML*/
    Q_INVOKABLE float resistanceAtResistor(int resNr){return resistors.at(resNr)->getValue();}
    /**Increase resistance for Resistor at resNr by 25 Ohm, invokable from QML*/
    Q_INVOKABLE void increaseResistanceAtResistor(int resNr){resistors.at(resNr)->setValue(resistors.at(resNr)->getValue()+25);}
    /**Decrease resistance for Resistor at resNr by 25 Ohm, invokable from QML*/
    Q_INVOKABLE void decreaseResistanceAtResistor(int resNr){if(resistors.at(resNr)->getValue()>25){resistors.at(resNr)->setValue(resistors.at(resNr)->getValue()-25);}}
    /**Get current trough Resistor at resNr, invokable from QML*/
    Q_INVOKABLE float getCurrentofResistor(int resNr){return resistors.at(resNr)->getCurrent();}
    /**Get angle of Resistor at resNr, invokable from QML*/
    Q_INVOKABLE int getAngleOfResistor(int resNr){return resistors.at(resNr)->getAngle();}
    /**Get XCoord of Resistor at resNr, invokable from QML*/
    Q_INVOKABLE int getXCoordOfResistor(int resNr){return resistors.at(resNr)->getXCoord();}
    /**Get YCoord Resistor at resNr, invokable from QML*/
    Q_INVOKABLE int getYCoordOfResistor(int resNr){return resistors.at(resNr)->getYCoord();}
    /**Get Node 1 of Resistor at resNr, invokable from QML*/
    Q_INVOKABLE int node1AtResistor(int resNr){return resistors.at(resNr)->getNode1();}
    /**Get Node 2 of Resistor at resNr, invokable from QML*/
    Q_INVOKABLE int node2AtResistor(int resNr){return resistors.at(resNr)->getNode2();}
    /**Add Resistor to resistors*/
    bool addResistor(std::shared_ptr<Resistor>& r);

    /**Get number of Sources, invokable from QML*/
    Q_INVOKABLE int getNumberOfSources(){return sources.size();}
    /**Get voltage at Source at sourceNr, invokable from QML*/
    Q_INVOKABLE float getVoltageAtSource(int sourceNr){return sources.at(sourceNr)->getValue();}
    /**Increase voltage at Source at sourceNr by 5 volt, invokable from QML*/
    Q_INVOKABLE void increaseVoltageAtSource(int sourceNr){sources.at(sourceNr)->setValue(sources.at(sourceNr)->getValue()+5);}
    /**Decrease voltage at Source at sourceNr by 5 volt, invokable from QML*/
    Q_INVOKABLE void decreaseVoltageAtSource(int sourceNr){if(sources.at(sourceNr)->getValue()>5){sources.at(sourceNr)->setValue(sources.at(sourceNr)->getValue()-5);}}
    /**Get current trough Source at sourceNr, invokable from QML*/
    Q_INVOKABLE float getCurrentofSource(int sourceNr){return sources.at(sourceNr)->getCurrent();}
    /**Get angle of Source at sourceNr, invokable from QML*/
    Q_INVOKABLE int getAngleOfSource(int soNr){return sources.at(soNr)->getAngle();}
    /**Get Xcoord of Source at sourceNr, invokable from QML*/
    Q_INVOKABLE int getXCoordOfSource(int soNr){return sources.at(soNr)->getXCoord();}
    /**Get Ycoord of Source at sourceNr, invokable from QML*/
    Q_INVOKABLE int getYCoordOfSource(int soNr){return sources.at(soNr)->getYCoord();}
    /**Get plusNode of Source at sourceNr, invokable from QML*/
    Q_INVOKABLE int nodePAtSource(int sourceNr){return sources.at(sourceNr)->getNodep();}
    /**Get minNode of Source at sourceNr, invokable from QML*/
    Q_INVOKABLE int nodeMAtSource(int sourceNr){return sources.at(sourceNr)->getNodem();}
    /**Add Source to sources*/
    bool addSource(std::shared_ptr<Source>& s);

    //Various functions for Wires, invokable from QML
    Q_INVOKABLE int getNumberOfWires(){return wires.size();}
    Q_INVOKABLE float getCurrentofWire(int wiNr){return wires.at(wiNr)->getCurrent();}
    Q_INVOKABLE int getAngleOfWire(int wiNr){return wires.at(wiNr)->getAngle();}
    Q_INVOKABLE int getXCoordOfWire(int wiNr){return wires.at(wiNr)->getXCoord();}
    Q_INVOKABLE int getYCoordOfWire(int wiNr){return wires.at(wiNr)->getYCoord();}
    Q_INVOKABLE int getNodeOfWire(int wiNr){return wires.at(wiNr)->getNode();}
    Q_INVOKABLE int getLengthOfWire(int wiNr){return wires.at(wiNr)->getLength();}
    bool addWire(std::shared_ptr<Wire> &w);

    //Various functions for Switches, invokable from QML
    Q_INVOKABLE int getNumberOfSwitches(){return switches.size();}
    Q_INVOKABLE bool isSwitchUp(int swNr){return (switches.at(swNr)->getUp());}
    Q_INVOKABLE int getAngleOfSwitch(int swNr){return switches.at(swNr)->getAngle();}
    Q_INVOKABLE int getXCoordOfSwitch(int swNr){return switches.at(swNr)->getXCoord();}
    Q_INVOKABLE int getYCoordOfSwitch(int swNr){return switches.at(swNr)->getYCoord();}
    Q_INVOKABLE int node1AtSwitch(int swNr){return switches.at(swNr)->getNode1();}
    Q_INVOKABLE int node2AtSwitch(int swNr){return switches.at(swNr)->getNode2();}
    Q_INVOKABLE void toggleSwitch(int swNr){switches.at(swNr)->toggleSwitch();}
    bool addSwitch(std::shared_ptr<Switch> &s);


    //Change the angle of resistors and switches, makes further calculations much easier
    void correctAngles();

    //Methodes for setting currents trough resistors, switches and wires
    void setCurrentsOfResistorsAndSwitches();
    void setCurrentsOfWires();
    void setCurrentsOfSwitchedWires();
    bool setCurrentsOfStrayWires();

    //Methodes for reading and writing file
    Q_INVOKABLE bool readFile();
    Q_INVOKABLE void writeBackToFile();

    //Read line of levelfile when a component is declared
    bool process_wire_line(QString& lijn);
    bool process_resistor_line(QString& lijn);
    bool process_source_line(QString& lijn);
    bool process_switch_line(QString& lijn);
    bool process_click_line(QString& lijn); //For other team

    //Set file path
    void setFileName(const QString &value);
    QString getFileName() const;


    //Getters and settters for vectors
    std::vector<std::shared_ptr<Wire>> getWires() const;
    std::vector<std::shared_ptr<Resistor> > getResistors() const;
    std::vector<std::shared_ptr<Source> > getSources() const;
    std::vector<std::shared_ptr<Switch> > getSwitches() const;

    //Empty all vectors
    void emptyVectors();



    //Getters and setters for level Goals
    int getTwoStar() const;
    void setTwoStar(int value);

    int getThreeStar() const;
    void setThreeStar(int value);


private:

    Calc();
    static std::shared_ptr<Calc> instance;

    //Internal solving method
    std::vector<float> computeNetwork(int nrOfNodes);


    //variables for circuit
    std::vector<float> sol;
    std::vector<std::shared_ptr<Source>> sources;
    std::vector<std::shared_ptr<Resistor>> resistors;
    std::vector<std::shared_ptr<Wire>> wires;
    std::vector<std::shared_ptr<Switch>> switches;
    QString fileName;


    //Things for compability with other team (game elements)
    int twoStar;
    int threeStar;
};

#endif // CALC_H
