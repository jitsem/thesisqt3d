import QtQuick 2.3 as QQ2
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Logic 2.0

//Main 3D class
Entity {
    id: world3D
    components: [forward_renderer,inputController]

    //Angle of camera
    property real cameraAngle:1

    //ViewPoint of camera
    property real x: 0
    property real y: 0
    property real z: 0

    //Zoomlevel
    property real zoomlevel: 50


    //Camera
    Camera {
        id: mainCamera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 60
        aspectRatio: _window.width / _window.height
        position: Qt.vector3d(zoomlevel*Math.sin(cameraAngle*180/Math.PI)+x*generator.sf, zoomlevel, -zoomlevel*Math.cos(cameraAngle*180/Math.PI)-z*generator.sf )
        viewCenter: Qt.vector3d(x*generator.sf, 0.0, -z*generator.sf)

    }


    //Render settings
    FrameGraph {
        id : forward_renderer

        Viewport {

            rect: Qt.rect(0.0, 0.0, 1.0, 1.0)
            clearColor: "darkslategrey"


            ClearBuffer {
                buffers : ClearBuffer.ColorDepthBuffer

            }

            CameraSelector {
                camera: mainCamera

            }
        }
    }


    //World generation
    property Entity generator: Generator{
        id:generator
    }

    //Groundplane for visisbility
    GroundPlane{

    }

    //Controller for zoom, rotate and other input
    property Entity inputController: InputController{
        id:inputController
    }


    //Set middle of camera
    function setCam(camx,camy){
        x = camx/generator.sf;
        z = camy/generator.sf;
        mainCamera.viewCenter =  Qt.vector3d(camx, 0.0, -camy);

    }


}

