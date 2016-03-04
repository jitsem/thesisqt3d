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
        text: "3D-Preview"
        font.pointSize: 24
        color: "red"
        anchors.horizontalCenter: parent.horizontalCenter
        style: Text.Raised
        styleColor: "yellow"
    }




    //Show nodal Solution

    Button{
        id:nodalInfo
        width: 50; height: 50
        anchors.top: rotateCameraLeft.bottom
        anchors.topMargin: 10
        anchors.left:parent.left
        iconSource: "qrc:/assets/icons/svg/question-button.svg"
        onClicked: solRect.visible = !solRect.visible
    }

    Rectangle{
        id:solRect
        width:320
        height: 100
        color: "steelblue"
        visible:false
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        Text{
            id:solText
            text: "The circuit has following solution: "
            font.pointSize: 10
            color: "white"
            style: Text.Raised
            styleColor: "white"

        }
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


    //Objects for saving edits
    Button{
        id:saveButton
        width: 50; height: 50
        anchors.top: zoomOut.bottom
        anchors.topMargin: 10
        anchors.right:parent.right
        iconSource: "qrc:/assets/icons/svg/inbox-symbol.svg"
        onClicked: {

            calculator.writeBackToFile();
            saveConfirm.visible = true;
            saveConfirmTimer.start();


        }


    }
    Rectangle{
        id:saveConfirm
        visible: false
        width: 100; height: 100
        color:"steelblue"
        anchors.bottom: parent.bottom
        anchors.right:parent.right
        anchors.bottomMargin: 15
        anchors.rightMargin: 15

        Image {
            width: 100; height: 100
            fillMode: Image.PreserveAspectFit
            smooth: true
            source: "qrc:/assets/icons/svg/mark-ribbon.svg"
        }

    }

    Timer{
        id:saveConfirmTimer
        interval: 4000;
        running: false;
        repeat: false;
        onTriggered:  saveConfirm.visible = false

    }




    //  Menu for changing Source & Resistor sizes and toggling switches, created after building level for firts time
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
        var switchMenuFactory = Qt.createComponent("qrc:/Qml/SwitchMenu.qml")
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

