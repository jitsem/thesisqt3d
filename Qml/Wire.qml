import Qt3D.Core 2.0
import Qt3D.Render 2.0
import QtQuick 2.2 as QQ2



Entity{
    //Positie variablen
    property real x: 0
    property real y: 0
    property real z: 0

    property real l: 1 //Lengte van draad
    property real orientationAngle: 90 //Hoek van draad

    components: [finmesh,fintrans]

    Entity{
        id:finmesh
        components: [wismesh,witrans]


        Entity{
            //basismodel draad
            id:wismesh
            components: [mesh, trans,mat]

            CylinderMesh {
                id:mesh
                radius: 0.1
                length: 1*l
            }
            Transform{
                id:trans
                translation: Qt.vector3d(0,-0.5*l,0)
            }

            PhongMaterial {
                id:mat
                diffuse: "yellow"
                ambient: "yellow"
                specular: "yellow"
                shininess: 0.2
            }

        }

        Transform{
            id:witrans
            matrix: {
                var m = Qt.matrix4x4();
                m.rotate(90,(Qt.vector3d(0, 0, 1)));
                m.scale(1);
                return m;
            }
        }

    }

    Transform{
        id:fintrans
        rotation: fromAxisAndAngle(Qt.vector3d(0,1,0),orientationAngle)
        translation: (Qt.vector3d(x, y, z))

    }



}



