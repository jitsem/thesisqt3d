import Qt3D.Core 2.0
import Qt3D.Logic 2.0
import QtQuick 2.3 as QQ2


Entity{
    id:root

    QQ2.QtObject{

        id:o
        property var compFactory

    }


    QQ2.Component.onCompleted: {
        o.compFactory=Qt.createComponent("qrc:/Qml/Comp.qml")

        for(var i=0;i<7;i++){
            var comp = o.compFactory.createObject(null,{"s":0.05,"x":i});
            comp.parent=root.parent;

        }
    }

}

