import QtQuick 2.5
import QtQuick.Controls 1.4

//Menu for editing a source or resistor value
Menu{
    id:root

    //Variable for Component Nr
    property real nr

    //Variable for ComponentType
    property string target: ""

    //Menu title
    title: target + nr

    //Signals for highlighting selected component
    onAboutToShow: {
        if(target === "resistor")
            toggleColorResistor(nr);
        else if(target === "source")
            toggleColorSource(nr);
    }

    onAboutToHide: {
        if(target === "resistor")
            toggleColorResistor(nr);
        else if(target === "source")
            toggleColorSource(nr);
    }

    //Menu Items and functions for adjusting components
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

