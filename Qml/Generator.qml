import Qt3D.Core 2.0
import Qt3D.Logic 2.0
import QtQuick 2.3 as QQ2


//Object dat verantwoordelijk is voor bouwen van circuit
Entity{
    id:root


    QQ2.QtObject{

        id:o
        property var sourceFactory
        property var resistorFactory


    }


    QQ2.Component.onCompleted: {

        //calculator.solveLevel(":/assets/inputfile.txt");

//        o.sourceFactory=Qt.createComponent("qrc:/Qml/Source.qml");
//        o.resistorFactory=Qt.createComponent("qrc:/Qml/Resistor.qml")


        //tijdelijk TODO delete

//        calculator.setAngleOfResistor(0,90);
//        calculator.setAngleOfResistor(1,0);
//        calculator.setAngleOfResistor(2,90);
//        calculator.setAngleOfResistor(3,90);
//        calculator.setAngleOfResistor(4, 90);

//        calculator.setXCoordOfResistor(0, 1);
//        calculator.setXCoordOfResistor(1, 0);
//        calculator.setXCoordOfResistor(2, 1);
//        calculator.setXCoordOfResistor(3, 6);
//        calculator.setXCoordOfResistor(4, 4);

//        calculator.setYCoordOfResistor(0, 5);
//        calculator.setYCoordOfResistor(1, 1);
//        calculator.setYCoordOfResistor(2, 3);
//        calculator.setYCoordOfResistor(3, 3);
//        calculator.setYCoordOfResistor(4, 4);

//        calculator.setAngleOfSource(0, 90);
//        calculator.setXCoordOfSource(0, 9);
//        calculator.setYCoordOfSource(0, 3);

//         calculator.setAngleOfSource(1, 90);
//         calculator.setXCoordOfSource(1, 4);
//         calculator.setYCoordOfSource(1, 3);





//        for(var i=0;i<calculator.getNumberOfSources();i++){

//            var negNode = calculator.nodeMAtSource(i);
//            var posNode= calculator.nodePAtSource(i);
//            var source = o.sourceFactory.createObject(null,{"s":calculator.voltageAtSource(i),"x":calculator.getXCoordOfSource(i)*3,
//                                                          "z":-calculator.getYCoordOfSource(i)*3, "y":calculator.voltageAtNode(negNode)});
//            source.parent=root.parent;


//        }

//        for(i=0;i<calculator.getNumberOfResistors();i++){

//            var firstNode = calculator.node1AtResistor(i);
//            var lastNode = calculator.node2AtResistor(i);
//            var resistor = o.resistorFactory.createObject(null,{"a":Math.atan2(3,(calculator.voltageAtNode(lastNode)-calculator.voltageAtNode(firstNode)))*180/Math.PI
//                                                              ,"s":0.01*calculator.resistanceAtResistor(i),"x":calculator.getXCoordOfResistor(i)*3,
//                                                              "z":-calculator.getYCoordOfResistor(i)*3,
//                                                              "y":calculator.voltageAtNode(lastNode), "orientationAngle":calculator.getAngleOfResistor(i)});
//            resistor.parent=root.parent;

//        }




//        //line for visibility
//        var line = o.resistorFactory.createObject(null,{"s":5000,"y":-1});

//        line.parent = root.parent;


//        setSol();


//    }

}


//o.sourceFactory=Qt.createComponent("qrc:/Qml/Source.qml");
//o.resistorFactory=Qt.createComponent("qrc:/Qml/Resistor.qml")
//o.wireFactory = Qt.createComponent("qrc:/Qml/Wire.qml")

//for (i = 0; i<components.size; i++)){


//    var elemenType = caclulator.getComponentAt(i).getType;  //Voor elk element in lijst (Resistor, Source, Draad)

//    Switch(type){
//        case(Resistor){
//            var resistor = o.resistorFactory.createObject(null,{"a":Math.atan2(3,(calculator.voltageAtNode(lastNode)-calculator.voltageAtNode(firstNode)))*180/Math.PI  ,"s":0.01*calculator.resistanceAtResistor(i),"x":calculator.getComponentAt(i).getXCoord(),"z": calculator.getComponentAt(i).getYCoord(), "y":calculator.voltageAtNode(lastNode)});
//            resistor.parent=root.parent;
//        }
//        case(Source){
//            var source = o.sourceFactory.createObject(null,{"s":calculator.voltageAtSource(i),"x":i*3, "y":calculator.voltageAtNode(negNode)});
//            source.parent=root.parent;
//        }
//        case(Wire){
//            var wire = o.wireFactory.createObject(null,{"a":Math.atan2(3,(calculator.voltageAtNode(lastNode)-calculator.voltageAtNode(firstNode)))*180/Math.PI  ,"s":0.01*calculator.resistanceAtResistor(i),"x":(i+calculator.getNumberOfSources())*3, "y":calculator.voltageAtNode(lastNode)});
//            wire.parent=wire.parent;
//        }
//        case(Open){
//            //Doe niks
//        }




//    }






//  }


//Vorige
//for(var i=0;i<calculator.getNumberOfSources();i++){

//    var negNode = calculator.nodeMAtSource(i);
//    var posNode= calculator.nodePAtSource(i);
//    var source = o.sourceFactory.createObject(null,{"s":calculator.voltageAtSource(i),"x":i*3, "y":calculator.voltageAtNode(negNode)});
//    source.parent=root.parent;

//}

//for(var i=0;i<calculator.getNumberOfResistors();i++){

//    var firstNode = calculator.node1AtResistor(i);
//    var lastNode = calculator.node2AtResistor(i);
//    var resistor = o.resistorFactory.createObject(null,{"a":Math.atan2(3,(calculator.voltageAtNode(lastNode)-calculator.voltageAtNode(firstNode)))*180/Math.PI  ,"s":0.01*calculator.resistanceAtResistor(i),"x":(i+calculator.getNumberOfSources())*3, "y":calculator.voltageAtNode(lastNode)});
//    resistor.parent=root.parent;

//}




////line for visibility
//var line = o.resistorFactory.createObject(null,{"s":5000,"y":-1});

//line.parent = root.parent;


//setSol();


//}

//}

}
