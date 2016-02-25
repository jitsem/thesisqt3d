import Qt3D.Core 2.0
import Qt3D.Input 2.0

//Controller voor input en keybindings
Entity{
    id:inputController



    KeyboardInput{
        id:keyInput
        controller: keyboardController
        onDownPressed: inputController.zoom(1);
        onUpPressed: inputController.zoom(-1)
    }

    KeyboardController {
        id: keyboardController

    }





    function cameraRotation(direction) {


        world3D.cameraAngle += direction/360;


    }

    function zoom(direction) {


        world3D.zoomlevel += direction;


    }


}
