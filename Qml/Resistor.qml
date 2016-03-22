import Qt3D.Core 2.0
import Qt3D.Render 2.0
import QtQuick 2.2 as QQ2


//Resistor Entity
Entity{
    id:node
    components: [finmesh,fintrans]

    //Thickness of Resistor, depends on resistance
    property real s: 1

    //Length of Resistor, depends on  voltage difference
    property real l: 1

    //Positional Variables
    property real x: 0
    property real y: 0
    property real z: 0

    //Angle in Y plane, depends on voltage difference
    property real a: 90

    //Angle in Z plane, depends on 2D orientation
    property real orientationAngle: 0

    //Resistor color
    property var resColor: "slateblue"

    //Entity without proper placement
    Entity{
        id:finmesh
        components: [resmesh,retrans]

        //Basic model Resistor
        Entity{

            id:resmesh
            components: [mesh, trans,mat]

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
                ambient: resColor
                specular: "darkslategray"
                shininess: 0.01
            }

        }

        Transform{
            id:retrans
            matrix: {
                var m = Qt.matrix4x4()
                m.rotate(a,(Qt.vector3d(0, 0, 1)));
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
        id:animateAngle
        target:node
        property:"a"
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
    function changeAngle(newValue){
        animateAngle.to = newValue;
        animateAngle.start();
    }
    function changeOrientationAngle(newValue){
        animateOrientationAngle.to = newValue;
        animateOrientationAngle.start();
    }



}



