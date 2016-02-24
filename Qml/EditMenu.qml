import QtQuick 2.5
import QtQuick.Controls 1.4

//Menu for editing a source

Menu{
    id:root
    property real sourceNr
    title:"source " + sourceNr
    MenuItem {
        text: "Increase Voltage"
        onTriggered: {

            calculator.increaseVoltageAtSource(sourceNr);
            world3D.generator.destroyLevel();
            world3D.generator.buildLevel();
        }
    }

    MenuItem {
        text: "Decrease Voltage"
        onTriggered: {

            calculator.decreaseVoltageAtSource(sourceNr);
            world3D.generator.destroyLevel();
            world3D.generator.buildLevel();
        }
    }

}

