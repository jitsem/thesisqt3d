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


    //Original start
    property real xstart:0

    //Startpoint for animation
    property real xbegin: 0

    //Endpoint for animation
    property real xend: 4

    //Duration of variable
    property real dur: 1000


    components: [mesh,trans,mat]

    SphereMesh{
        id:mesh
        radius:s*3
    }

    Transform
    {
        id:trans
        translation: (Qt.vector3d(x, y, z))
    }
    PhongMaterial {
        id:mat
        diffuse: "steelblue"
        ambient: "gainsboro"
        specular: "blue"
        shininess: 0.2

    }

    QQ2.SequentialAnimation{

        id:currentPath
        running: true
        loops: QQ2.Animation.Infinite
        QQ2.NumberAnimation {

            target: electron
            property: "x"
            duration: electron.dur*Math.abs(xbegin-xend)
            from: electron.xbegin
            to: electron.xend
        }
        QQ2.NumberAnimation {

            target: electron
            property: "x"
            duration: electron.dur*Math.abs(xstart-xbegin)
            from: electron.xstart
            to: electron.xbegin
        }

    }

    QQ2.NumberAnimation{
        id:animateSize
        target:electron
        property:"s"
        duration: 1000
    }

    function changeSize(newValue){
        animateSize.to = newValue;
        animateSize.start();
    }

    function stopAnimation(){
        currentPath.stop();
    }
    function startAnimation(){
        currentPath.start();
    }

}
