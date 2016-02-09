import Qt3D.Core 2.0
import Qt3D.Render 2.0
import QtQuick 2.2 as QQ2



Entity{
    id:node
    property real s: 1
    property real x: 0
    property real y: 0
    property real z: 0

    components: [mesh,trans]

    SphereMesh {
        id:mesh
        radius:5
        rings:64
        slices:64

    }

    Transform{
        id:trans

        scale: s
        translation: Qt.vector3d(x, y, z)

    }

}
