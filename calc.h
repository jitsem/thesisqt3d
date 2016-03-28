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


/**Class where all major calculations and I/O happen*/
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

    /**Get number of resistors, invokable from QML*/
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

    /**Get number of sources, invokable from QML*/
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

    /**Get number of wires, invokable from QML*/
    Q_INVOKABLE int getNumberOfWires(){return wires.size();}
    /**Get current trough Wire at wiNr, invokable from QML*/
    Q_INVOKABLE float getCurrentofWire(int wiNr){return wires.at(wiNr)->getCurrent();}
    /**Get angle of Wire at wiNr, invokable from QML*/
    Q_INVOKABLE int getAngleOfWire(int wiNr){return wires.at(wiNr)->getAngle();}
    /**Get XCoord of Wire at wiNr, invokable from QML*/
    Q_INVOKABLE int getXCoordOfWire(int wiNr){return wires.at(wiNr)->getXCoord();}
    /**Get YCoord of Wire at wiNr, invokable from QML*/
    Q_INVOKABLE int getYCoordOfWire(int wiNr){return wires.at(wiNr)->getYCoord();}
    /**Get Node of Wire at wiNr, invokable from QML*/
    Q_INVOKABLE int getNodeOfWire(int wiNr){return wires.at(wiNr)->getNode();}
    /**Get length of Wire at wiNr, invokable from QML*/
    Q_INVOKABLE int getLengthOfWire(int wiNr){return wires.at(wiNr)->getLength();}
    /**Add Wire to wires*/
    bool addWire(std::shared_ptr<Wire> &w);

    /**Get number of wires, invokable from QML*/
    Q_INVOKABLE int getNumberOfSwitches(){return switches.size();}
    /**Get state of Switch at swNr, invokable from QML*/
    Q_INVOKABLE bool isSwitchUp(int swNr){return (switches.at(swNr)->getUp());}
    /**Get angle of Switch at swNr, invokable from QML*/
    Q_INVOKABLE int getAngleOfSwitch(int swNr){return switches.at(swNr)->getAngle();}
    /**Get XCoord of Switch at swNr, invokable from QML*/
    Q_INVOKABLE int getXCoordOfSwitch(int swNr){return switches.at(swNr)->getXCoord();}
    /**Get YCoord of Switch at swNr, invokable from QML*/
    Q_INVOKABLE int getYCoordOfSwitch(int swNr){return switches.at(swNr)->getYCoord();}
    /**Get Node 1 of Switch at swNr, invokable from QML*/
    Q_INVOKABLE int node1AtSwitch(int swNr){return switches.at(swNr)->getNode1();}
    /**Get Node 2 of Switch at swNr, invokable from QML*/
    Q_INVOKABLE int node2AtSwitch(int swNr){return switches.at(swNr)->getNode2();}
    /**Toggle Switch at swNr, invokable from QML*/
    Q_INVOKABLE void toggleSwitch(int swNr){switches.at(swNr)->toggleSwitch();}
    /**Add Switch to switches*/
    bool addSwitch(std::shared_ptr<Switch> &s);


    /**Standardize the angles of resistors and switches, makes current-calculations much easier*/
    void correctAngles();
    /**Set currents trough resistors, switches and wires*/
    void setCurrentsOfResistorsAndSwitches();
    /**Set currents trough wires, based on other components*/
    void setCurrentsOfWires();
    /**Set currents of wires, which are not directly connected to other components*/
    bool setCurrentsOfStrayWires();

    /**Read in file*/
    bool readFile();
    /**Write changes to file*/
    void writeBackToFile();

    /**Read line of levelfile for a Wire component*/
    bool process_wire_line(QString& lijn);
    /**Read line of levelfile for a Resistor component*/
    bool process_resistor_line(QString& lijn);
    /**Read line of levelfile for a Source component*/
    bool process_source_line(QString& lijn);
    /**Read line of levelfile for a Switch component*/
    bool process_switch_line(QString& lijn);
    /**Read line of levelfile for the click Goal*/
    bool process_click_line(QString& lijn); //For other team

    /**Setter for file path*/
    void setFileName(const QString &value);
    /**Getter for file path*/
    QString getFileName() const;


    /**Get wires*/
    std::vector<std::shared_ptr<Wire>> getWires() const;
    /**Get resistors*/
    std::vector<std::shared_ptr<Resistor> > getResistors() const;
    /**Get sources*/
    std::vector<std::shared_ptr<Source> > getSources() const;
    /**Get switches*/
    std::vector<std::shared_ptr<Switch> > getSwitches() const;

    /**Empty all vectors*/
    void emptyVectors();


    /**Getter for twoStarGoal*/
    int getTwoStar() const;
    /**Getter for twoStarGoal*/
    void setTwoStar(int value);
    /**Getter for threeStarGoal*/
    int getThreeStar() const;
    /**Getter for threeStarGoal*/
    void setThreeStar(int value);


private:

    /**Constructor for Calc*/
    Calc();
    /**Instance pointer for Calc, makes sure there is only one instance of this class*/
    static std::shared_ptr<Calc> instance;
    /**Calculates voltage of each nodes, based on components in circuit*/
    std::vector<float> computeNetwork(int nrOfNodes);
    /**Vector with nodeVoltage*/
    std::vector<float> sol;
    /**Vector with all objects of type Source*/
    std::vector<std::shared_ptr<Source>> sources;
    /**Vector with all objects of type Resistors*/
    std::vector<std::shared_ptr<Resistor>> resistors;
    /**Vector with all objects of type Wire*/
    std::vector<std::shared_ptr<Wire>> wires;
    /**Vector with all objects of type Switch*/
    std::vector<std::shared_ptr<Switch>> switches;
    /**File path*/
    QString fileName;
    /**Two star goal*/
    int twoStar;
    /**Three star goal*/
    int threeStar;
};

#endif // CALC_H
