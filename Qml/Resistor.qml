import Qt3D.Core 2.0
import Qt3D.Render 2.0
import QtQuick 2.2 as QQ2



Entity{
    id:node
    property real s: 1
    property real x: 0
    property real y: 0
    property real z: 0
    property real a: 90
    property real orientationAngle: 0
    components: [resmesh,retrans]

    Entity{
        id:resmesh

        components: [mesh, trans,mat]



        CylinderMesh {
            id:mesh
            radius: 0.5
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
                        m.translate(Qt.vector3d(x, y, z))
                        m.rotate(a,(Qt.vector3d(0, 0, 1)))
                        m.rotate(orientationAngle,Qt.vector3d(0, 1, 0))
                        m.scale(1)
                        return m
                    }

        scale3D : Qt.vector3d(0.5, 1*s, 0.5)




    }




}



