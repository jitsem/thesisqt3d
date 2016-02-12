import QtQuick 2.5
import QtQuick.Scene3D 2.0
import QtQuick.Controls 1.4


Item {
    id:appView

    Scene3D{
        id:scene3D
        anchors.fill: parent
        focus:true
        aspects: "input"


        World3D{
            id:world3D

        }

    }



    Text{
        id:titleText
        text: " WELCOME TO THE GAME"
        font.pointSize: 24
        color: "red"
        anchors.horizontalCenter: parent.horizontalCenter

        style: Text.Raised
        styleColor: "white"
    }

    Text{
        id:solText
        text: "The circuit has following solution: "
        font.pointSize: 10
        color: "white"
        anchors.verticalCenter: parent.verticalCenter

        style: Text.Raised
        styleColor: "white"
    }


    function setSol(){
        solText.text+= calculator.numberOfNodes();
        solText.text+= " nodes: \n";
         for (var i=0;i<calculator.numberOfNodes();i++){
           solText.text += "voltage at node nr " +i +" " +(calculator.voltageAtNode(i)) +" volt.\n";
         }

}














}



