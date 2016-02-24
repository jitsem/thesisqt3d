import QtQuick 2.5
import QtQuick.Controls 1.4

//Main Menu
Item {
    id:startMenu

    Image {
        id: backGround
        source: "qrc:/assets/textures/ground.jpg"
        anchors.fill: parent
    }



    Button {
        id: playButton
        text: "Play"
        width: parent.width/2; height: parent.height/6
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height/6
        onClicked: {

            startMenu.visible = false;
            circuit.visible = true;



        }

    }
    Button {
        id: buildButton
        text: "Build"
        width: parent.width/2; height: parent.height/6
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: playButton.bottom
        anchors.topMargin: parent.height/6
    }
    Button {
        id: exitButton
        text:"Exit"
        width: parent.width/2; height: parent.height/6
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: buildButton.bottom
        anchors.topMargin: parent.height/6
        onClicked: Qt.quit()
    }

    CircuitView{
        id:circuit
        visible: false
    }


}
