import Qt3D.Core 2.0
import Qt3D.Logic 2.0
import QtQuick 2.3 as QQ2


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
            var source = o.sourceFactory.createObject(null,{"s":0.1*calculator.voltageAtSource(i),"x":3*calculator.nodePAtSource(i)});
            source.parent=root.parent;

        }

        for(var i=0;i<calculator.getNumberOfResistors();i++){
            var resistor = o.resistorFactory.createObject(null,{"s":0.01*calculator.resistanceAtResistor(i),"x":3*calculator.node1AtResistor(i), "z":3});
            resistor.parent=root.parent;

        }

        setSol();

    }

}

