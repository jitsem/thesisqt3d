import Qt3D.Core 2.0
import Qt3D.Logic 2.0
import QtQuick 2.3 as QQ2


//Object dat verantwoordelijk is voor bouwen van circuitsc
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

        calculator.solveLevel(":/assets/inputfile.txt");

        o.sourceFactory=Qt.createComponent("qrc:/Qml/Source.qml");
        o.resistorFactory=Qt.createComponent("qrc:/Qml/Resistor.qml");
        o.wireFactory=Qt.createComponent("qrc:/Qml/Wire.qml");



        //tijdelijk TODO delete

        calculator.setAngleOfResistor(0,0);
        calculator.setAngleOfResistor(1,90);
        calculator.setAngleOfResistor(2,0);
        calculator.setAngleOfResistor(3,180);
        calculator.setAngleOfResistor(4, 0);

        calculator.setXCoordOfResistor(0, 1);
        calculator.setXCoordOfResistor(1, 0);
        calculator.setXCoordOfResistor(2, 1);
        calculator.setXCoordOfResistor(3, 6);
        calculator.setXCoordOfResistor(4, 4);

        calculator.setYCoordOfResistor(0, 5);
        calculator.setYCoordOfResistor(1, 1);
        calculator.setYCoordOfResistor(2, 3);
        calculator.setYCoordOfResistor(3, 3);
        calculator.setYCoordOfResistor(4, 4);

        calculator.setAngleOfSource(0, 90);
        calculator.setXCoordOfSource(0, 9);
        calculator.setYCoordOfSource(0, 3);

        calculator.setAngleOfSource(1, 90);
        calculator.setXCoordOfSource(1, 4);
        calculator.setYCoordOfSource(1, 3);





        for(var i=0;i<calculator.getNumberOfSources();i++){

            var negNode = calculator.nodeMAtSource(i);
            var posNode= calculator.nodePAtSource(i);
            var source = o.sourceFactory.createObject(null,{"s":calculator.voltageAtSource(i),
                                                            "x":calculator.getXCoordOfSource(i)*o.sf,
                                                            "z":-calculator.getYCoordOfSource(i)*o.sf,
                                                            "y":calculator.voltageAtNode(negNode)});
            source.parent=root.parent;
            o.sources[o.sources.length]=source;


        }

        for(i=0;i<calculator.getNumberOfResistors();i++){


            var firstNode = calculator.node1AtResistor(i);
            var lastNode = calculator.node2AtResistor(i);

            //Hoek van de weerstand
            var angle = Math.atan2(o.sf,(calculator.voltageAtNode(lastNode)-calculator.voltageAtNode(firstNode)));

            //Lengte van de weerstand
            var length = Math.abs(((calculator.voltageAtNode(lastNode)-calculator.voltageAtNode(firstNode)))/Math.cos(angle));

            var resistor = o.resistorFactory.createObject(null,{"a":angle*180/Math.PI,
                                                                "l":length,
                                                                "s":calculator.resistanceAtResistor(i),
                                                                "x":calculator.getXCoordOfResistor(i)*o.sf,
                                                                "z":-calculator.getYCoordOfResistor(i)*o.sf,
                                                                "y":calculator.voltageAtNode(lastNode),
                                                                "orientationAngle":calculator.getAngleOfResistor(i)});

            resistor.parent=root.parent;
            o.resistors[o.resistors.length]=resistor;

        }




        //add wires TODO make automatisch
        var wire1 = o.wireFactory.createObject(null,{"x":0*o.sf,"z":-2*o.sf, "y":12.6, "l":o.sf});
        wire1.parent=root.parent;

        var wire2 = o.wireFactory.createObject(null,{"x":0*o.sf,"z":-3*o.sf, "y":12.6, "orientationAngle":0, "l":o.sf});
        wire2.parent=root.parent;

        var wire3 = o.wireFactory.createObject(null,{"x":2*o.sf,"z":-3*o.sf,"y":23,"orientationAngle":0, "l":o.sf*2});

        wire3.parent=root.parent;



        var wire5 = o.wireFactory.createObject(null,{"x":4*o.sf,"z":-3*o.sf,"y":8.04,"orientationAngle":0, "l":o.sf});

        wire5.parent=root.parent;

        var wire5 = o.wireFactory.createObject(null,{"x":6*o.sf,"z":-3*o.sf,"y":20,"orientationAngle":0, "l":o.sf*3});

        wire5.parent=root.parent;


        console.log("number of sources, resistors", o.sources.length, o.resistors.length);

    }

}




