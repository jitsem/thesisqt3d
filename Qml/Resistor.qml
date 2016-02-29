import Qt3D.Core 2.0
import Qt3D.Render 2.0
import QtQuick 2.2 as QQ2



Entity{
    id:node
    property real s: 1 //bepaald dikte vd weerstand, afhankelijk van weestandswaarde
    property real l: 1 //bepaald lengte vd weerstand, afhankelijk van spanning over weerstand

    //Variablen voor posistie
    property real x: 0
    property real y: 0
    property real z: 0

    //Variable voor hoek.
    property real a: 90 //Hoek volgens z as,bepaald door spanning over weerstand
    property real orientationAngle: 0 //Hoek volgens y as, bepaald door plaatsing weerstand

    components: [finmesh,fintrans]

    Entity{
        //Weerstand met juiste waardes, zonder plaatsing
        id:finmesh
        components: [resmesh,retrans]

        Entity{
            //Basismodel weestand
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
                diffuse: "green"
                ambient: "green"
                specular: "blue"
                shininess: 0.2
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



