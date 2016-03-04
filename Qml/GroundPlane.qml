import Qt3D.Core 2.0
import Qt3D.Render 2.0


//Grondvlak om 3D-zicht te verbeteren
Entity {
    id: root


    PlaneMesh {
        id: groundMesh
        width: 300
        height: width
        meshResolution: Qt.size(2, 2)

    }

    Transform {
        id: groundTransform
        translation: Qt.vector3d(0,-5,0);

    }

    DiffuseSpecularMapMaterial {
        id: groundMaterial
        ambient: "darkslategrey"
        diffuse: "qrc:/assets/textures/ground.png"


    }

    components: [
        groundMesh,
        groundTransform,
        groundMaterial
    ]
}


