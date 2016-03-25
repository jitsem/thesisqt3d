import QtQuick 2.5
import QtQuick.Scene3D 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

//Overlay with ui elements
Item {
    id:appView


    //Contains 3D-world
    Scene3D{
        id:scene3D
        anchors.fill: parent
        focus:true
        aspects: ["logic", "input"]


        World3D{
            id:world3D

        }

    }

    //Title text
    Text{
        id:titleText
        text: "3D-Preview"
        font.pointSize: 24
        color: "orange"
        anchors.horizontalCenter: parent.horizontalCenter
        style: Text.Raised
        styleColor: "gold"
    }

    //Default style for buttons
    Component{
        id:butStyle

        ButtonStyle{

            background:     Rectangle {
                width: parent.width<parent.height?parent.width:parent.height
                height: width
                border.width: control.activeFocus ? 2 : 1
                border.color: "#888"
                radius: width*0.5
                color: control.pressed ? "#ccc" : "#eee"


            }
        }
    }


    //Objects for showing nodal Solution
    Button{
        id:nodalInfo
        width:  mainWindow.getGridSize(); height:  mainWindow.getGridSize()
        anchors.top: rotateCameraLeft.bottom
        anchors.topMargin: 10
        anchors.left:parent.left
        iconSource: "qrc:/assets/icons/svg/question-button.svg"
        onClicked: solRect.visible = !solRect.visible
        style: butStyle
    }


    Rectangle{
        id:solRect
        width:mainWindow.getGridSize()*5
        height: calculator.numberOfNodes()*mainWindow.getGridSize()/3.7
        color: "steelblue"
        visible:false
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
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
            solText.text += "Voltage @ node " +i +" " +(calculator.voltageAtNode(i).toFixed(2)) +" Volt.\n";
        }

    }


    //Functions, timers and buttons for camera Rotation and zoom
    Button{
        id:rotateCameraRight
        width:  mainWindow.getGridSize(); height:  mainWindow.getGridSize()
        anchors.top:parent.top
        anchors.right:parent.right
        iconSource: "qrc:/assets/icons/svg/turn-right.svg"
        style: butStyle



    }


    Button{
        id:rotateCameraLeft
        width:  mainWindow.getGridSize(); height:  mainWindow.getGridSize()
        anchors.top:parent.top
        anchors.left:parent.left
        iconSource: "qrc:/assets/icons/svg/turn-left-circle.svg"
        style: butStyle


    }

    Button{
        id:zoomIn
        width:  mainWindow.getGridSize(); height:  mainWindow.getGridSize()
        anchors.right: parent.right
        anchors.top: rotateCameraRight.bottom
        anchors.topMargin: 10
        iconSource: "qrc:/assets/icons/svg/triangle-pointing-up.svg"
        style: butStyle

    }


    Button{
        id:zoomOut
        width:  mainWindow.getGridSize(); height:  mainWindow.getGridSize()
        anchors.right: parent.right
        anchors.top: zoomIn.bottom
        anchors.topMargin: 5
        iconSource: "qrc:/assets/icons/svg/turn-triangle.svg"
        style: butStyle
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



    //Exit Menu
    Button{
        id:exitButton
        width:  mainWindow.getGridSize(); height:  mainWindow.getGridSize()
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
        anchors.right:parent.right
        iconSource: "qrc:/assets/icons/png/direction.png"
        onClicked: {

            mainWindow.delete3D();
        }
        style: butStyle
    }







    //Menu and buttons for changing Source & Resistor sizes
    Button{
        id:sourceEdit
        width:  mainWindow.getGridSize(); height:  mainWindow.getGridSize()
        anchors.top: nodalInfo.bottom
        anchors.topMargin: 10
        anchors.left:parent.left
        iconSource: "qrc:/assets/source_small.png"
        onClicked: sourceMenu.popup()
        style: butStyle

    }
    Menu{
        id:sourceMenu
    }

    Button{
        id:resistorEdit
        width:  mainWindow.getGridSize(); height:  mainWindow.getGridSize()
        anchors.top: sourceEdit.bottom
        anchors.topMargin: 10
        anchors.left:parent.left
        iconSource: "qrc:/assets/res_small.png"
        onClicked:resistorMenu.popup()
        style: butStyle

    }

    Menu{
        id:resistorMenu
    }


    //Constructor for editMenus
    function makeEditMenu() {


        var menuFactory = Qt.createComponent("qrc:/Qml/EditMenu.qml");
        var switchMenuFactory = Qt.createComponent("qrc:/Qml/SwitchMenu.qml");

        for (var i = 0; i < world3D.generator.sources.length; i++) {
            var menu = menuFactory.createObject(appView,{"target":"source", "nr":i});
            sourceMenu.insertItem(i,menu);

        }
        for (var i = 0; i < world3D.generator.resistors.length; i++) {
            var menu = menuFactory.createObject(appView,{"target":"resistor", "nr":i});
            resistorMenu.insertItem(i,menu);

        }

    }


    //Toggle Source color, called in menus
    function toggleColorSource(i){
        if(world3D.generator.sources[i].sourceColor === "firebrick")
            world3D.generator.sources[i].sourceColor = "yellow";
        else
            world3D.generator.sources[i].sourceColor = "firebrick";
    }

    //Toggle Resistor color
    function toggleColorResistor(i){
        if(world3D.generator.resistors[i].resColor === "slateblue")
            world3D.generator.resistors[i].resColor = "lime";
        else
            world3D.generator.resistors[i].resColor = "slateblue";
    }

    //Toggle Switch color
    function toggleColorSwitch(i){
        if(world3D.generator.switches[i].switchColor === "orange")
            world3D.generator.switches[i].switchColor = "lime";
        else
            world3D.generator.switches[i].switchColor = "orange";
    }



}

