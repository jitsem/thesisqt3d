import Qt3D.Core 2.0
import Qt3D.Logic 2.0
import QtQuick 2.3 as QQ2


//Object dat verantwoordelijk is voor bouwen van circuits
Entity{
    id:root

    //Size of each coordinate step
    property real sf:5

    //Arrays for components
    property var sources:[]
    property var resistors :[]
    property var wires: []
    property var switches: []

    //Stuff for making components
    QQ2.QtObject{
        id:o

        //Variables for spawning objects
        property var sourceFactory
        property var resistorFactory
        property var wireFactory
        property var switchFactory

    }

    QQ2.Component.onCompleted: {


        buildLevel(); //Build circuit
        makeEditMenu(); //Build Edit menu


    }

    function buildLevel(){

        //Assing Qml files to factoris
        o.sourceFactory=Qt.createComponent("qrc:/Qml/Source.qml");
        o.resistorFactory=Qt.createComponent("qrc:/Qml/Resistor.qml");
        o.wireFactory=Qt.createComponent("qrc:/Qml/Wire.qml");
        o.switchFactory=Qt.createComponent("qrc:/Qml/Switch.qml");

        //For each type of component, build all of that type

        //Build Sources
        for(var i=0;i<calculator.getNumberOfSources();i++){

            //Negative node
            var negNode = calculator.nodeMAtSource(i);

            //Assign correct params and make
            var source = o.sourceFactory.createObject(null,{"s":calculator.getVoltageAtSource(i),
                                                          "x":calculator.getXCoordOfSource(i)*root.sf,
                                                          "z":-calculator.getYCoordOfSource(i)*root.sf,
                                                          "y":calculator.voltageAtNode(negNode)});
            source.parent=root.parent;
            root.sources[i]=source;
        }

        //Build Resistors
        for(i=0;i<calculator.getNumberOfResistors();i++){


            var minVolt = Math.min(calculator.voltageAtNode(calculator.node1AtResistor(i)),calculator.voltageAtNode(calculator.node2AtResistor(i)));
            var maxVolt = Math.max(calculator.voltageAtNode(calculator.node1AtResistor(i)),calculator.voltageAtNode(calculator.node2AtResistor(i)));

            //Angle in Y plane
            var angle = Math.atan2(root.sf,(minVolt-maxVolt));

            //Length depending on above angle
            var length = Math.max(1*root.sf,Math.abs(((maxVolt-minVolt))/Math.cos(angle)));

            //Assign correct params and make
            var resistor = o.resistorFactory.createObject(null,{"a":(angle*180/Math.PI),
                                                              "l":length,
                                                              "s":calculator.resistanceAtResistor(i),
                                                              "x":calculator.getXCoordOfResistor(i)*root.sf,
                                                              "z":-calculator.getYCoordOfResistor(i)*root.sf,
                                                              "y":minVolt,
                                                              "orientationAngle":90*(calculator.getAngleOfResistor(i)-1)});

            resistor.parent=root.parent;
            root.resistors[i]=resistor;
        }


        //Build wires
        for(i=0;i<calculator.getNumberOfWires();i++){


            //Assign correct params and make
            var wire = o.wireFactory.createObject(null,{"x":calculator.getXCoordOfWire(i)*root.sf,
                                                      "z":-calculator.getYCoordOfWire(i)*root.sf,
                                                      "y":calculator.voltageAtNode(calculator.getNodeOfWire(i)),
                                                      "l":calculator.getLengthOfWire(i)*root.sf,
                                                      "orientationAngle":90*(calculator.getAngleOfWire(i)-1),
                                                      "eSize": calculator.getCurrentofWire(i),
                                                      "sf":root.sf});
            wire.parent=root.parent;
            root.wires[i]=wire;
        }

        //Build switches
        for(i=0;i<calculator.getNumberOfSwitches();i++){

            //Lowest voltage
            var minVolt = Math.min(calculator.voltageAtNode(calculator.node1AtSwitch(i)),calculator.voltageAtNode(calculator.node2AtSwitch(i)));

            //Assign correct params and make
            var swi = o.switchFactory.createObject(root,{"switchNr":i,
                                                       "s":70,
                                                       "l": 1*root.sf,
                                                       "x":calculator.getXCoordOfSwitch(i)*root.sf,
                                                       "z":-calculator.getYCoordOfSwitch(i)*root.sf,
                                                       "y":minVolt,
                                                       "orientationAngle":90*(calculator.getAngleOfSwitch(i)-1)});
            swi.parent=root.parent;
            root.switches[i]=swi;
        }

        setSol();    //show nodal solution on screen, for debugging
        setMiddle(); //set camera position

    }

    //Recalculate some values and adjust some components, similar to above
    function redrawLevel(){
        calculator.solveLevel();

        for(var i=0;i<sources.length;i++){

            sources[i].changeSize(calculator.getVoltageAtSource(i));
            sources[i].changeHeight(calculator.voltageAtNode(calculator.nodeMAtSource(i)));
            sources[i].sourceColor = "firebrick"

        }

        for(var i=0;i<resistors.length;i++){

            var minVolt = Math.min(calculator.voltageAtNode(calculator.node1AtResistor(i)),calculator.voltageAtNode(calculator.node2AtResistor(i)));
            var maxVolt = Math.max(calculator.voltageAtNode(calculator.node1AtResistor(i)),calculator.voltageAtNode(calculator.node2AtResistor(i)));
            var angle = Math.atan2(root.sf,(minVolt-maxVolt));
            var length = Math.max(1*root.sf,Math.abs(((maxVolt-minVolt))/Math.cos(angle)));

            resistors[i].changeAngle(angle*180/Math.PI);
            resistors[i].changeLength(length);
            resistors[i].changeSize(calculator.resistanceAtResistor(i));
            resistors[i].changeHeight(minVolt);
            resistors[i].changeOrientationAngle(90*(calculator.getAngleOfResistor(i)-1));
            resistors[i].x=calculator.getXCoordOfResistor(i)*root.sf;
            resistors[i].z=-calculator.getYCoordOfResistor(i)*root.sf;
            resistors[i].resColor = "slateblue"

        }

        for(var i=0;i<wires.length;i++){

            wires[i].changeHeight(calculator.voltageAtNode(calculator.getNodeOfWire(i)));
            wires[i].eSize = calculator.getCurrentofWire(i);
            wires[i].adjustElectrons();

        }

        for(var i=0;i<switches.length;i++){
            var minVolt = Math.min(calculator.voltageAtNode(calculator.node1AtSwitch(i)),calculator.voltageAtNode(calculator.node2AtSwitch(i)));

            if(calculator.isSwitchUp(i)){
                switches[i].changeLength(0.1*root.sf);
                switches[i].changeHeight(minVolt)
            }
            else{
                switches[i].changeLength(1*root.sf);
                switches[i].changeHeight(minVolt)
            }
            switches[i].switchColor = "orange"

        }
        setSol();
    }


    //Timer and function for preventing more than one switch transition at once
    QQ2.Timer {
        id:clickPrev
        interval: 1200
        running: false;
        repeat: false
    }

    function clickPrevent(){
        if(!clickPrev.running){
            clickPrev.start();
            return false;
        }

        return true;
    }

    //Function for getting middle of circuit
    function setMiddle(){

        var MaxX = 1;
        var MinX= 100000;

        for(var i=0;i<wires.length;i++){
            MaxX= Math.max(MaxX,wires[i].x);
            MinX=Math.min(MinX,wires[i].x);

        }

        var MaxY = 1;
        var MinY= 100000;

        for(var i=0;i<wires.length;i++){
            MaxY= Math.max(MaxY,-wires[i].z);
            MinY=Math.min(MinY,-wires[i].z);

        }

        setCam((((MaxX-MinX)/2)+MinX), (((MaxY-MinY)/2)+MinY));

    }
}




