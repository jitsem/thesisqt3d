import Qt3D.Core 2.0
import Qt3D.Logic 2.0
import QtQuick 2.3 as QQ2


//Object dat verantwoordelijk is voor bouwen van circuits
Entity{
    id:root


    QQ2.QtObject{

        id:o

        //global vars
        property var sf:5 //size of each coordinate step: scaleFactor

        //Variables for spawning objects
        property var sourceFactory
        property var resistorFactory
        property var wireFactory

        //arrays for components
        property var sources:[]
        property var resistors :[]
        property var wires: []




    }


    QQ2.Component.onCompleted: {
        buildLevel(); //Bouw Ciruict
        setSol();    //show nodal solution on screen, for debugging


    }

    function buildLevel(){

        calculator.readFile(":/assets/Levels/inputfile_1.sj");
        calculator.solveLevel();


        o.sourceFactory=Qt.createComponent("qrc:/Qml/Source.qml");
        o.resistorFactory=Qt.createComponent("qrc:/Qml/Resistor.qml");
        o.wireFactory=Qt.createComponent("qrc:/Qml/Wire.qml");

        for(var i=0;i<calculator.getNumberOfSources();i++){


            var negNode = calculator.nodeMAtSource(i);
            var posNode= calculator.nodePAtSource(i);
            var source = o.sourceFactory.createObject(null,{"s":calculator.voltageAtSource(i),
                                                          "x":calculator.getXCoordOfSource(i)*o.sf,
                                                          "z":-calculator.getYCoordOfSource(i)*o.sf,
                                                          "y":calculator.voltageAtNode(negNode)});
            source.parent=root.parent;
            o.sources[o.sources.length]=source;

            console.log("Current trough source: ", i , calculator.getCurrentofSource(i));


        }

        //TODO Probleem met angle oplossen

        for(i=0;i<calculator.getNumberOfResistors();i++){


            var minVolt = Math.min(calculator.voltageAtNode(calculator.node1AtResistor(i)),calculator.voltageAtNode(calculator.node2AtResistor(i)));
            var maxVolt = Math.max(calculator.voltageAtNode(calculator.node1AtResistor(i)),calculator.voltageAtNode(calculator.node2AtResistor(i)));


            //Hoek van de weerstand
            var angle = Math.atan2(o.sf,(minVolt-maxVolt));


            //Lengte van de weerstand
            var length = Math.abs(((maxVolt-minVolt))/Math.cos(angle));





            //Loop voor hoek



            var resistor = o.resistorFactory.createObject(null,{"a":(angle*180/Math.PI),
                                                              "l":length,
                                                              "s":calculator.resistanceAtResistor(i),
                                                              "x":calculator.getXCoordOfResistor(i)*o.sf,
                                                              "z":-calculator.getYCoordOfResistor(i)*o.sf,
                                                              "y":minVolt,
                                                              "orientationAngle":90*(calculator.getAngleOfResistor(i)-1)});

            resistor.parent=root.parent;
            o.resistors[o.resistors.length]=resistor;
            console.log("Current trough resistor: ", i ,calculator.getCurrentofResistor(i));

        }



        //add wires TODO make automatisch

        for(i=0;i<calculator.getNumberOfWires();i++){



            var wire = o.wireFactory.createObject(null,{"x":calculator.getXCoordOfWire(i)*o.sf,
                                                         "z":-calculator.getYCoordOfWire(i)*o.sf,
                                                         "y":calculator.voltageAtNode(calculator.getNodeOfWire(i)),
                                                         "l":calculator.getLengthOfWire(i)*o.sf,
                                                         "orientationAngle":90*(calculator.getAngleOfWire(i)-1),
                                                         "eSize": calculator.getCurrentofWire(i)});
            wire.parent=root.parent;
            o.wires[o.wires.length]=wire;
            console.log("Current trough Wire at pos : ", calculator.getXCoordOfWire(i),calculator.getYCoordOfWire(i),calculator.getCurrentofWire(i));

        }

        console.log("number of sources, resistors", o.sources.length, o.resistors.length);

    }

}




