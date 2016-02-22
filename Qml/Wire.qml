import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Logic 2.0
import QtQuick 2.2 as QQ2



Entity{
    id:root
    //Positie variablen
    property real x: 0
    property real y: 0
    property real z: 0

    property real l: 1 //Lengte van draad
    property real orientationAngle: 90 //Hoek van draad

    //Variables for spawning electrons
    property bool done: false
    property real framecount
    property real eSize

    components: [finmesh,fintrans,electronSpawner]

    Entity{
        id:finmesh
        components: [wismesh,witrans]


        Entity{
            //basismodel draad
            id:wismesh
            components: [mesh, trans,mat]

            CylinderMesh {
                id:mesh
                radius: 0.02
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




    //Stuff for spawning electrons
    QQ2.QtObject{
        id:o
        property var electronFactory

    }
    QQ2.Component.onCompleted: {
        o.electronFactory=Qt.createComponent("qrc:/Qml/Electron.qml");
        root.done = true;
    }

    LogicComponent{
        id: electronSpawner

        onFrameUpdate:
        {
            spawnElectron();

        }
    }

    function spawnElectron(){
        root.framecount++;
        if(!root.done)
            return;

        if(root.framecount >= 60){
            if(root.eSize>0)
                var electron = o.electronFactory.createObject(null,{"xend":root.l, "dur":750*root.l, "s": Math.abs(root.eSize)});
            else
                var electron = o.electronFactory.createObject(null,{"xend":0, "xbegin": root.l, "dur":750*root.l, "s": Math.abs(root.eSize)});
            electron.parent=root;
            root.framecount=0;
        }
    }







}



