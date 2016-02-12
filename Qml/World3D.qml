import QtQuick 2.3 as QQ2
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Logic 2.0


Entity {
    id: world3D
    property real cameraAngle1: 1
    property real cameraAngle2: 1





    Configuration  {
        controlledCamera: mainCamera
    }


    Camera {
        id: mainCamera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 60
        aspectRatio: 16/9
        nearPlane : 0.1
        farPlane : 1000.0
        position: Qt.vector3d( 0*cameraAngle2, 10.0, 50.0*cameraAngle1 )
        upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
    }




    FrameGraph {
        id : forward_renderer
        activeFrameGraph : ForwardRenderer {
            camera: mainCamera
            clearColor: "black"
        }
    }

    components: [forward_renderer]


    Generator{
        id:generator
    }



}
