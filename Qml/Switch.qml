import Qt3D.Core 2.0
import Qt3D.Render 2.0
import QtQuick 2.2 as QQ2


//Switch Entity
Entity{
    id:node
    components: [finmesh,fintrans]

    //Thickness of switch
    property real s: 1

    //Lenght of switch
    property real l: 1

    //Positional variables
    property real x: 0
    property real y: 0
    property real z: 0

    //Angle in Z plane
    property real orientationAngle: 0

    //Ambient color for highlighting
    property var switchColor: "orange"

    //Nr for switching
    property real switchNr


    //Switch without correct posistion
    Entity{
        id:finmesh
        components: [resmesh,retrans]

        //Basic model Switch
        Entity{
            id:resmesh
            components: [mesh, trans,mat, picker]

            CylinderMesh {
                id:mesh
                radius: 1
                length: 1
            }

            Transform{

                id:trans
                translation: Qt.vector3d(0, -0.5, 0)

            }

            PhongMaterial {
                id:mat
                diffuse: "darkslategray"
                ambient: switchColor
                specular: "darkslategray"
                shininess: 0.2
            }

            //Picker for toggling switch
            ObjectPicker{
                id:picker
                onClicked: {
                    if(!node.parent.generator.clickPrevent()){
                        calculator.toggleSwitch(switchNr);
                        world3D.generator.redrawLevel();

                    }
                }

            }

        }
        Transform{
            id:retrans
            matrix: {
                var m = Qt.matrix4x4()
                m.rotate(90,(Qt.vector3d(0, 0, 1)));
                m.scale(1);
                return m
            }
            scale3D: Qt.vector3d(0.005*s,l,0.005*s)
        }
    }

    Transform{
        id:fintrans
        rotation: fromAxisAndAngle(Qt.vector3d(0,1,0),orientationAngle)
        translation: (Qt.vector3d(x, y, z))
    }


    //Animation-functions and objects
    QQ2.NumberAnimation{
        id:animateSize
        target: node
        property: "s"
        duration: 1000

    }

    QQ2.NumberAnimation{
        id:animateHeight
        target:node
        property:"y"
        duration: 1000
    }
    QQ2.NumberAnimation{
        id:animateLength
        target: node
        property: "l"
        duration: 1000

    }


    QQ2.NumberAnimation{
        id:animateOrientationAngle
        target:node
        property:"orientationAngle"
        duration: 1000
    }




    function changeSize(newValue){
        animateSize.to = newValue;
        animateSize.start();

    }
    function changeHeight(newValue){
        animateHeight.to = newValue;
        animateHeight.start();
    }
    function changeLength(newValue){
        animateLength.to = newValue;
        animateLength.start();

    }

    function changeOrientationAngle(newValue){
        animateOrientationAngle.to = newValue;
        animateOrientationAngle.start();
    }
}

