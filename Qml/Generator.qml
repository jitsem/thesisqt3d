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

            var negNode = calculator.nodeMAtSource(i); //TODO wegdoen, is maar tijdelijk
            if(negNode === 0){
                negNode = calculator.numberOfNodes();
            }

            var source = o.sourceFactory.createObject(null,{"s":calculator.voltageAtSource(i),"x":calculator.nodePAtSource(i)*2, "y":calculator.voltageAtNode(negNode-1)});

            source.parent=root.parent;

        }

        for(var i=0;i<calculator.getNumberOfResistors();i++){

            var firstNode = calculator.node2AtResistor(i); //TODO wegdoen, is maar tijdelijk

            if(firstNode === 0){
                firstNode = calculator.numberOfNodes();
            }
            var resistor = o.resistorFactory.createObject(null,{"s":0.01*calculator.resistanceAtResistor(i),"x":calculator.node1AtResistor(i)*2, "y":calculator.voltageAtNode(firstNode-1)});
            resistor.parent=root.parent;

        }

        setSol();

    }

}

