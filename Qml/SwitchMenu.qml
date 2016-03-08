import QtQuick 2.5
import QtQuick.Controls 1.4

//Menu for editing a switch
//TODO verwijderen als touch deftig werkt
Menu{
    id:root
    property real nr
    title: "Switch " + nr
    onAboutToShow: {

        toggleColorSwitch(nr);
    }
    onAboutToHide: {

        toggleColorSwitch(nr);
    }


    MenuItem {
        text: "Toggle"
        onTriggered: {
            calculator.toggleSwitch(nr)
            world3D.generator.redrawLevel();
        }



    }
}

