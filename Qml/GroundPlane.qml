import Qt3D.Core 2.0
import Qt3D.Render 2.0


//Grondvlak om 3D-zicht te verbeteren
Entity {
    id: root


    PlaneMesh {
        id: groundMesh
        width: 500
        height: width
        meshResolution: Qt.size(2, 2)

    }

    Transform {
        id: groundTransform
        translation: Qt.vector3d(0,-50,0);

    }

    PhongMaterial {
        id: groundMaterial
        ambient: "darkslategrey"
        diffuse: "black"
        shininess:50


    }

    components: [
        groundMesh,
        groundTransform,
        groundMaterial
    ]
}


