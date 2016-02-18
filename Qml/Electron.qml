import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Logic 2.0
import QtQuick 2.2 as QQ2



Entity{
    id:electron

    //PositieVariablen
    property real x: 0
    property real y: 0
    property real z: 0

    //size of electron
    property real s: 1



    //Startpoint for animation
    property real xbegin: 0

    //Endpoint for animation
    property real xend: 4

    //Duration of variable
    property real dur: 10000


    components: [mesh,trans]

    SphereMesh{
        id:mesh
        radius:s*3
    }

    Transform
    {
        id:trans
        translation: (Qt.vector3d(x, y, z))
    }

    QQ2.NumberAnimation {
            target: electron
            running: true
            loops: 1

            property: "x"
            duration: electron.dur
            from: 0
            to: electron.xend
        }
    LogicComponent{
        id:electronDeleter
        onFrameUpdate: {
            if(electron.x == xend){
                electron.destroy();
            }
        }
    }
}
