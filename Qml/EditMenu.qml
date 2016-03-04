import QtQuick 2.5
import QtQuick.Controls 1.4

//Menu for editing a source

Menu{
    id:root
    property real nr
    property string target: ""
    title: target + nr
    MenuItem {
        text: "Increase"
        onTriggered: {

            increase();
            world3D.generator.redrawLevel();
        }
    }

    MenuItem {
        text: "Decrease"
        onTriggered: {

            decrease();
            world3D.generator.redrawLevel();
        }
    }

    function increase(){
        if(target === "resistor")
        {

            calculator.increaseResistanceAtResistor(nr);

        }
        else if(target === "source"){

            calculator.increaseVoltageAtSource(nr);

        }


    }

    function decrease(){
        if(target === "resistor")
        {
            calculator.decreaseResistanceAtResistor(nr)


        }
        else if(target === "source"){

            calculator.decreaseVoltageAtSource(nr);

        }


    }

}

