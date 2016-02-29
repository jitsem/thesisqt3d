import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import QtQuick 2.2 as QQ2





Entity{
    id:source

    property real s: 1 //Grote van bron, bepaald door spanning


    //Positie variablen
    property real x: 0
    property real y: 0
    property real z: 0

    //AnimatieVariablen
    property real s_new:1 //Nieuwe waarde voor animatie
    property bool animationRunning: false // Animatie in overgang

    components: [somesh,sotrans]

    Entity{
        //Basismodel bron
        id:somesh
        components: [mesh,trans,mat]

        CuboidMesh {
            id:mesh
            yzMeshResolution: Qt.size(2, 2)
            xzMeshResolution: Qt.size(2, 2)
            xyMeshResolution: Qt.size(2, 2)
        }

        Transform{
            id:trans
            translation: Qt.vector3d(0, 0.5, 0)
        }

        PhongMaterial {
            id:mat
            diffuse: "red"
            ambient: "red"
            specular: "blue"
            shininess: 0.2
        }
    }

    Transform{
        id:sotrans
        translation: Qt.vector3d(x, y, z)
        scale3D : Qt.vector3d(2, 1*s, 2)

    }

    //Animation-functions and objects
    QQ2.NumberAnimation{
        id:animateSize
        target: source
        property: "s"
        duration: 1000

    }

    QQ2.NumberAnimation{
        id:animateHeight
        target:source
        property:"y"
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


}

