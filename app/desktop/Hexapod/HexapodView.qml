import QtQuick
import QtQuick3D
import "models/qt"

Item{
    id: root
    function setLegAngles(leg, coxa, femur, tibia){
        hexapod.setLegAngles(leg, coxa, femur, tibia)
    }
    View3D{
        anchors.fill: parent

        environment: SceneEnvironment{
            backgroundMode: SceneEnvironment.Color
            clearColor: "#202020"
        }

        Hexapod{
            id: hexapod
        }

        Component.onCompleted: {
            for(var i = 1; i < 7; i++){
                hexapod.setLegAngles(i, 0, 60, 30)
            }
        }

        Node{
            id: tiltedOrbit
//            eulerRotation.x: -20

            Node{
                id: orbit

                NumberAnimation on eulerRotation.y{
                    from: 0
                    to: 360
                    duration: 30000
                    loops: Animation.Infinite
                    running: true
                }

                PerspectiveCamera{
                    id: camera
                    z: 500
                    y: -100
                }
            }
        }

        DirectionalLight{
            eulerRotation.x: -45
            eulerRotation.y: -30
            brightness: 2.0
        }
    }
}
