import QtQuick 2.5
import QtQuick.Scene3D 2.0
import QtQuick.Controls 1.4


Item {
    id:root

    Scene3D{
        id:scene3D
        anchors.fill: parent
        focus:true


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














}



