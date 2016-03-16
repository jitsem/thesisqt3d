import Qt3D.Core 2.0
import Qt3D.Logic 2.0
import QtQuick 2.3 as QQ2


//Object dat verantwoordelijk is voor bouwen van circuits
Entity{
    id:root

    //size of each coordinate step
    property real sf:5
    //arrays for components
    property var sources:[]
    property var resistors :[]
    property var wires: []
    property var switches: []

    QQ2.QtObject{

        id:o


        //Variables for spawning objects
        property var sourceFactory
        property var resistorFactory
        property var wireFactory
        property var switchFactory

    }

    QQ2.Component.onCompleted: {


        buildLevel(); //Bouw Ciruict
        makeEditMenu();


    }

    function buildLevel(){

        calculator.solveLevel();
        o.sourceFactory=Qt.createComponent("qrc:/Qml/Source.qml");
        o.resistorFactory=Qt.createComponent("qrc:/Qml/Resistor.qml");
        o.wireFactory=Qt.createComponent("qrc:/Qml/Wire.qml");
        o.switchFactory=Qt.createComponent("qrc:/Qml/Switch.qml");


        for(var i=0;i<calculator.getNumberOfSources();i++){

            var negNode = calculator.nodeMAtSource(i);
            var source = o.sourceFactory.createObject(null,{"s":calculator.getVoltageAtSource(i),
                                                          "x":calculator.getXCoordOfSource(i)*root.sf,
                                                          "z":-calculator.getYCoordOfSource(i)*root.sf,
                                                          "y":calculator.voltageAtNode(negNode)});
            source.parent=root.parent;
            root.sources[i]=source;

            console.log("Current trough source: ", i , calculator.getCurrentofSource(i));


        }

        for(i=0;i<calculator.getNumberOfResistors();i++){


            var minVolt = Math.min(calculator.voltageAtNode(calculator.node1AtResistor(i)),calculator.voltageAtNode(calculator.node2AtResistor(i)));
            var maxVolt = Math.max(calculator.voltageAtNode(calculator.node1AtResistor(i)),calculator.voltageAtNode(calculator.node2AtResistor(i)));


            //Hoek van de weerstand
            var angle = Math.atan2(root.sf,(minVolt-maxVolt));


            //Lengte van de weerstand
            var length = Math.abs(((maxVolt-minVolt))/Math.cos(angle));



            var resistor = o.resistorFactory.createObject(null,{"a":(angle*180/Math.PI),
                                                              "l":length,
                                                              "s":calculator.resistanceAtResistor(i),
                                                              "x":calculator.getXCoordOfResistor(i)*root.sf,
                                                              "z":-calculator.getYCoordOfResistor(i)*root.sf,
                                                              "y":minVolt,
                                                              "orientationAngle":90*(calculator.getAngleOfResistor(i)-1)});

            resistor.parent=root.parent;
            root.resistors[i]=resistor;
            console.log("Current trough resistor: ", i ,calculator.getCurrentofResistor(i));

        }



        for(i=0;i<calculator.getNumberOfWires();i++){



            var wire = o.wireFactory.createObject(null,{"x":calculator.getXCoordOfWire(i)*root.sf,
                                                      "z":-calculator.getYCoordOfWire(i)*root.sf,
                                                      "y":calculator.voltageAtNode(calculator.getNodeOfWire(i)),
                                                      "l":calculator.getLengthOfWire(i)*root.sf,
                                                      "orientationAngle":90*(calculator.getAngleOfWire(i)-1),
                                                      "eSize": calculator.getCurrentofWire(i),
                                                      "sf":root.sf});
            wire.parent=root.parent;
            root.wires[i]=wire;
            console.log("Current trough Wire at pos : ", calculator.getXCoordOfWire(i),calculator.getYCoordOfWire(i),calculator.getCurrentofWire(i));

        }

        for(i=0;i<calculator.getNumberOfSwitches();i++){

            var minVolt = Math.min(calculator.voltageAtNode(calculator.node1AtSwitch(i)),calculator.voltageAtNode(calculator.node2AtSwitch(i)));

            var swi = o.switchFactory.createObject(null,{"switchNr":i,
                                                       "s":70,
                                                       "l": 1*root.sf,
                                                       "x":calculator.getXCoordOfSwitch(i)*root.sf,
                                                       "z":-calculator.getYCoordOfSwitch(i)*root.sf,
                                                       "y":minVolt,
                                                       "orientationAngle":90*(calculator.getAngleOfSwitch(i)-1)});
            swi.parent=root.parent;
            root.switches[i]=swi;
            console.log("Switch: ", calculator.getXCoordOfSwitch(i),calculator.getYCoordOfSwitch(i));

        }

        console.log("number of sources, resistors", root.sources.length, root.resistors.length);

        setSol();    //show nodal solution on screen, for debugging
        setMiddle(); //set camera position

    }


    function redrawLevel(){
        calculator.solveLevel();

        for(var i=0;i<sources.length;i++){

            sources[i].changeSize(calculator.getVoltageAtSource(i));
            sources[i].changeHeight(calculator.voltageAtNode(calculator.nodeMAtSource(i)));

            //TODO tijdelijk
            sources[i].sourceColor = "firebrick"

        }

        for(var i=0;i<resistors.length;i++){

            var minVolt = Math.min(calculator.voltageAtNode(calculator.node1AtResistor(i)),calculator.voltageAtNode(calculator.node2AtResistor(i)));
            var maxVolt = Math.max(calculator.voltageAtNode(calculator.node1AtResistor(i)),calculator.voltageAtNode(calculator.node2AtResistor(i)));


            //Hoek van de weerstand
            var angle = Math.atan2(root.sf,(minVolt-maxVolt));


            //Lengte van de weerstand
            var length = Math.abs(((maxVolt-minVolt))/Math.cos(angle));

            resistors[i].changeAngle(angle*180/Math.PI);
            resistors[i].changeLength(length);
            resistors[i].changeSize(calculator.resistanceAtResistor(i));
            resistors[i].changeHeight(minVolt);
            resistors[i].changeOrientationAngle(90*(calculator.getAngleOfResistor(i)-1));
            resistors[i].x=calculator.getXCoordOfResistor(i)*root.sf;
            resistors[i].z=-calculator.getYCoordOfResistor(i)*root.sf;

            //TODO tijdelijk
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

            //TODO tijdelijk
            switches[i].switchColor = "orange"

        }


        setSol();
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

        setCam( (((MaxX-MinX)/2)+MinX), (((MaxY-MinY)/2)+MinY));

    }
    }




