import QtQuick 2.5
import QtQuick.Scene3D 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

//Overlay with ui elements
Item {
    id:appView

    PinchArea {
        anchors.fill: parent
        pinch.minimumScale: 0.1
        pinch.maximumScale: 10
        onPinchUpdated: {
            if(pinch.previousScale<pinch.scale)
                world3D.inputController.zoom(-1);
            else
                world3D.inputController.zoom(1)
            zoomSlide.value=105-world3D.zoomlevel;
        }

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
            iconSource: "qrc:/assets/icons/info.png"
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



        //Functions, timers and buttons for camera Rotation and zoom
        Button{
            id:rotateCameraRight
            width:  mainWindow.getGridSize(); height:  mainWindow.getGridSize()
            anchors.top:parent.top
            anchors.right:parent.right
            iconSource: "qrc:/assets/icons/rotate-right.png"
            style: butStyle



        }


        Button{
            id:rotateCameraLeft
            width:  mainWindow.getGridSize(); height:  mainWindow.getGridSize()
            anchors.top:parent.top
            anchors.left:parent.left
            iconSource: "qrc:/assets/icons/rotate-left.png"
            style: butStyle


        }


        Slider{
            id:zoomSlide
            height: parent.height-(mainWindow.getGridSize()*3)
            anchors.top:rotateCameraLeft.bottom
            anchors.right: parent.right
            anchors.topMargin: 40
            anchors.rightMargin: 20
            orientation: Qt.Vertical
            value: 55
            minimumValue: 0
            maximumValue: 100
            stepSize: 0
            updateValueWhileDragging: true
            onValueChanged: {
                world3D.zoomlevel=105-value;
            }
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



        //Exit Menu
        Button{
            id:exitButton
            width:  mainWindow.getGridSize(); height:  mainWindow.getGridSize()
            anchors.bottom: parent.bottom
            anchors.topMargin: 10
            anchors.right:parent.right
            iconSource: "qrc:/assets/icons/exit.png"
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
            iconSource: "qrc:/assets/icons/source_small.png"
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
            iconSource: "qrc:/assets/icons/res_small.png"
            onClicked:resistorMenu.popup()
            style: butStyle

        }

        Menu{
            id:resistorMenu
        }






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

    //Set voltage info
    function setSol(){
        solText.text="The circuit has following solution: ";
        solText.text+= calculator.numberOfNodes();
        solText.text+= " nodes: \n";
        for (var i=0;i<calculator.numberOfNodes();i++){
            solText.text += "Voltage @ node " +i +" " +(calculator.voltageAtNode(i).toFixed(2)) +" Volt.\n";
        }

    }

}

