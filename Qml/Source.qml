import Qt3D.Core 2.0
import Qt3D.Render 2.0
import QtQuick 2.2 as QQ2



Entity{
    id:node
    property real s: 1
    property real x: 0
    property real y: 0
    property real z: 0

    components: [somesh,sotrans]


    Entity{

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
        translation: Qt.vector3d(x, y+0.5, z)
        scale3D : Qt.vector3d(0.5, 1*s, 0.5)

    }





}
