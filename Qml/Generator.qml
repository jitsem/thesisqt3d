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

        calculator.solveLevel(":/assets/inputfile.txt");

        o.sourceFactory=Qt.createComponent("qrc:/Qml/Source.qml");
        o.resistorFactory=Qt.createComponent("qrc:/Qml/Resistor.qml")

        for(var i=0;i<calculator.getNumberOfSources();i++){

            var negNode = calculator.nodeMAtSource(i);
            var posNode= calculator.nodePAtSource(i);

            var source = o.sourceFactory.createObject(null,{"s":calculator.voltageAtSource(i),"x":i*3, "y":calculator.voltageAtNode(negNode)});

            source.parent=root.parent;

        }

        for(var i=0;i<calculator.getNumberOfResistors();i++){

            var firstNode = calculator.node1AtResistor(i);
            var lastNode = calculator.node2AtResistor(i);


            var resistor = o.resistorFactory.createObject(null,{"a":Math.atan2(3,(calculator.voltageAtNode(lastNode)-calculator.voltageAtNode(firstNode)))*180/Math.PI  ,"s":0.01*calculator.resistanceAtResistor(i),"x":(i+calculator.getNumberOfSources())*3, "y":calculator.voltageAtNode(lastNode)});
            resistor.parent=root.parent;

        }




        //line for visibility
        var line = o.resistorFactory.createObject(null,{"s":5000,"y":-1});

        line.parent = root.parent;


        setSol();


    }

}

