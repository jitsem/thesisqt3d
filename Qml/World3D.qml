import QtQuick 2.3 as QQ2
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Logic 2.0


Entity {
    id: world3D
    property real cameraAngle

    //CiruitMiddle
    property real x: 4.5
    property real y: 0
    property real z: -4.5

    //Zoomlevel
    property real zoomlevel: 40


    //Camera
    Camera {
        id: mainCamera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 60
        aspectRatio: _window.width / _window.height
        position: Qt.vector3d(zoomlevel*Math.sin(cameraAngle*180/Math.PI)+x*generator.sf, zoomlevel, -zoomlevel*Math.cos(cameraAngle*180/Math.PI)+z*generator.sf )
        viewCenter: Qt.vector3d(x*generator.sf, 0.0, z*generator.sf)
    }



    //Render settings
    FrameGraph {
        id : forward_renderer
        Viewport {
            rect: Qt.rect(0.0, 0.0, 1.0, 1.0)
            clearColor: "black"

            ClearBuffer {
                buffers : ClearBuffer.ColorDepthBuffer

            }

            CameraSelector {
                camera: mainCamera

            }
        }
    }


    components: [forward_renderer,inputController]

    //World generation
    property Entity generator: Generator{
        id:generator
    }

    GroundPlane{

    }


    property Entity inputController: InputController{
        id:inputController
    }



}
