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
    property real eSize

    //global scale factor
    property real sf:1


    //list of electrons
    property var electrons: []
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
                radius: 0.02
                length: 1*l
            }
            Transform{
                id:trans
                translation: Qt.vector3d(0,-0.5*l,0)
            }

            PhongMaterial {
                id:mat
                diffuse: "gold"
                ambient: "gold"
                specular: "white"
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
        spawnElectrons();
    }


    function spawnElectrons(){

        for(var i=0;i<2*root.l/root.sf;i++){

            if(root.eSize>0)
                var electron = o.electronFactory.createObject(null,{"xend":root.l, "xstart":0 , "xbegin":i/2*root.sf , "s": Math.abs(root.eSize)});
            else
                var electron = o.electronFactory.createObject(null,{"xend":0, "xbegin": i/2*root.sf,"xstart":root.l, "s": Math.abs(root.eSize)});
            electron.parent=root;
            electrons[i]=electron;

        }
    }

    function destroyElectrons(){
        for(var i=0;i<electrons.length;i++){
            electrons[i].destroy();
        }
    }


    //Animation-functions and objects

    QQ2.NumberAnimation{
        id:animateHeight
        target:root
        property:"y"
        duration: 1000
    }


    function changeHeight(newValue){
        animateHeight.to = newValue;
        animateHeight.start();
    }

    function adjustElectrons(){

        for(var i = 0 ; i < electrons.length ; i++){

            electrons[i].stopAnimation();
            electrons[i].changeSize(Math.abs(root.eSize));
            if(root.eSize>0){
                electrons[i].xend=root.l;
                electrons[i].xstart = 0;
            }else{
                electrons[i].xend=0;
                electrons[i].xstart =root.l;
            }
            electrons[i].startAnimation();
        }
    }








}



