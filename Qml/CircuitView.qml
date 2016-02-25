import QtQuick 2.5
import QtQuick.Scene3D 2.0
import QtQuick.Controls 1.4

//Overlay met Qtquick elementen
Item {
    id:appView


    Scene3D{
        id:scene3D
        anchors.fill: parent
        focus:true
        aspects: ["logic", "input"]


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




    //Show nodal Solution

    Button{
        id:nodalInfo
        width: 50; height: 50
        anchors.top: rotateCameraLeft.bottom
        anchors.topMargin: 10
        anchors.left:parent.left
        iconSource: "qrc:/assets/icons/svg/question-button.svg"
        onClicked: solText.visible = !solText.visible
    }


    Text{
        id:solText
        visible: false
        text: "The circuit has following solution: "
        font.pointSize: 10
        color: "white"
        anchors.verticalCenter: parent.verticalCenter
        style: Text.Raised
        styleColor: "white"
    }


    function setSol(){
        solText.text="The circuit has following solution: ";
        solText.text+= calculator.numberOfNodes();
        solText.text+= " nodes: \n";
        for (var i=0;i<calculator.numberOfNodes();i++){
            solText.text += "voltage at node nr " +i +" " +(calculator.voltageAtNode(i)) +" volt.\n";
        }

    }


    //Functions, timers and buttons for camera Rotation and zoom:

    Button{
        id:rotateCameraRight
        width: 50; height: 50
        anchors.top:parent.top
        anchors.right:parent.right
        iconSource: "qrc:/assets/icons/svg/turn-right.svg"



    }


    Button{
        id:rotateCameraLeft
        width: 50; height: 50
        anchors.top:parent.top
        anchors.left:parent.left
        iconSource: "qrc:/assets/icons/svg/turn-left-circle.svg"


    }

    Button{
        id:zoomIn
        width: 50; height: 50
        anchors.right: parent.right
        anchors.top: rotateCameraRight.bottom
        anchors.topMargin: 10
        iconSource: "qrc:/assets/icons/svg/triangle-pointing-up.svg"

    }


    Button{
        id:zoomOut
        width: 50; height: 50
        anchors.right: parent.right
        anchors.top: zoomIn.bottom
        anchors.topMargin: 5
        iconSource: "qrc:/assets/icons/svg/turn-triangle.svg"
    }

    Timer {
        id:cameraLeft
        interval: 40;
        running: rotateCameraLeft.pressed;
        repeat: true
        onTriggered:  world3D.inputController.cameraRotation(1)

    }

    Timer {
        id:cameraRight
        interval: 40;
        running: rotateCameraRight.pressed;
        repeat: true
        onTriggered:  world3D.inputController.cameraRotation(-1)

    }

    Timer {
        id:zoomInTimer
        interval: 40;
        running: zoomIn.pressed;
        repeat: true
        onTriggered:  world3D.inputController.zoom(-1)

    }

    Timer {
        id:zoomOutTimer
        interval: 40;
        running: zoomOut.pressed;
        repeat: true
        onTriggered:  world3D.inputController.zoom(1)

    }



    //Menu for changing sources sizes, created after building level for firts time
    Button{
        id:sourceEdit
        width: 50; height: 50
        anchors.top: nodalInfo.bottom
        anchors.topMargin: 10
        anchors.left:parent.left
        iconSource: "qrc:/assets/icons/svg/battery-power.svg"
        menu:sourceMenu

    }
    Menu{
        id:sourceMenu
    }


    Button{
        id:resistorEdit
        width: 50; height: 50
        anchors.top: sourceEdit.bottom
        anchors.topMargin: 10
        anchors.left:parent.left
        iconSource: "qrc:/assets/icons/svg/battery-power.svg"
        menu:resistorMenu

    }

    Menu{
        id:resistorMenu
    }


    function makeEditMenu() {

        var menuFactory = Qt.createComponent("qrc:/Qml/EditMenu.qml");
        for (var i = 0; i < world3D.generator.sources.length; i++) {
            var menu = menuFactory.createObject(appView,{"target":"source", "nr":i});
            sourceMenu.insertItem(i+1,menu);

        }
        for (var i = 0; i < world3D.generator.resistors.length; i++) {
            var menu = menuFactory.createObject(appView,{"target":"resistor", "nr":i});
            resistorMenu.insertItem(i+1,menu);

        }

    }

}

