import QtQuick
import QtQuick3D

Node {
    id: node

    // Resources
    PrincipledMaterial {
        id: principledMaterial
        metalness: 1
        roughness: 1
        alphaMode: PrincipledMaterial.Opaque
    }

    // Nodes:
    Node {
        id: root
        Node {
            id: center
            Model {
                id: leg_LF_
                position: Qt.vector3d(-72.0989, 6.21684, -144.296)
                source: "meshes/leg_LF.mesh"
                materials: principledMaterial
                Model {
                    id: coxa_LF_
                    position: Qt.vector3d(-8.26954, 11.5208, -31.4787)
                    scale: Qt.vector3d(1, 1, 1)
                    source: "meshes/coxa_LF.mesh"
                    materials: principledMaterial
                    Model {
                        id: femur_LF_
                        position: Qt.vector3d(54.1114, -3.07857, -32.92)
                        source: "meshes/femur_LF.mesh"
                        materials: principledMaterial
                        Model {
                            id: tibia_LF_
                            position: Qt.vector3d(91.5057, -0.350215, -0.110187)
                            source: "meshes/tibia_LF.mesh"
                            materials: principledMaterial
                        }
                    }
                }
            }
            Model {
                id: leg_LM_
                position: Qt.vector3d(-72.0989, 6.21684, 25.4739)
                source: "meshes/leg_LM.mesh"
                materials: principledMaterial
                Model {
                    id: coxa_LM_
                    position: Qt.vector3d(-8.26954, 11.5208, -31.4787)
                    scale: Qt.vector3d(1, 1, 1)
                    source: "meshes/coxa_LM.mesh"
                    materials: principledMaterial
                    Model {
                        id: femur_LM_
                        position: Qt.vector3d(54.1114, -3.07857, -32.92)
                        source: "meshes/femur_LM.mesh"
                        materials: principledMaterial
                        Model {
                            id: tibia_LM_
                            position: Qt.vector3d(91.5057, -0.350218, -0.110206)
                            source: "meshes/tibia_LM.mesh"
                            materials: principledMaterial
                        }
                    }
                }
            }
            Model {
                id: leg_LR_
                position: Qt.vector3d(-72.0989, 6.21684, 195.048)
                source: "meshes/leg_LR.mesh"
                materials: principledMaterial
                Model {
                    id: coxa_LR_
                    position: Qt.vector3d(-8.26954, 11.5208, -31.4787)
                    scale: Qt.vector3d(1, 1, 1)
                    source: "meshes/coxa_LR.mesh"
                    materials: principledMaterial
                    Model {
                        id: femur_LR_
                        position: Qt.vector3d(54.1114, -3.07857, -32.92)
                        source: "meshes/femur_LR.mesh"
                        materials: principledMaterial
                        Model {
                            id: tibia_LR_
                            position: Qt.vector3d(91.5057, -0.350219, -0.110167)
                            source: "meshes/tibia_LR.mesh"
                            materials: principledMaterial
                        }
                    }
                }
            }
            Model {
                id: leg_RF_
                position: Qt.vector3d(67.3305, 6.21684, -210.401)
                source: "meshes/leg_RF.mesh"
                materials: principledMaterial
                Model {
                    id: coxa_RF_
                    position: Qt.vector3d(-8.26953, 11.5208, -31.4786)
                    scale: Qt.vector3d(1, 1, 1)
                    source: "meshes/coxa_RF.mesh"
                    materials: principledMaterial
                    Model {
                        id: femur_RF_
                        position: Qt.vector3d(54.1114, -3.07857, -32.92)
                        source: "meshes/femur_RF.mesh"
                        materials: principledMaterial
                        Model {
                            id: tibia_RF_
                            position: Qt.vector3d(91.5057, -0.350218, -0.110171)
                            source: "meshes/tibia_RF.mesh"
                            materials: principledMaterial
                        }
                    }
                }
            }
            Model {
                id: leg_RM_
                position: Qt.vector3d(67.3305, 6.21684, -40.4855)
                source: "meshes/leg_RM.mesh"
                materials: principledMaterial
                Model {
                    id: coxa_RM_
                    position: Qt.vector3d(-8.26953, 11.5208, -31.4787)
                    scale: Qt.vector3d(1, 1, 1)
                    source: "meshes/coxa_RM.mesh"
                    materials: principledMaterial
                    Model {
                        id: femur_RM_
                        position: Qt.vector3d(54.1114, -3.07857, -32.92)
                        source: "meshes/femur_RM.mesh"
                        materials: principledMaterial
                        Model {
                            id: tibia_RM_
                            position: Qt.vector3d(91.5057, -0.350217, -0.110198)
                            source: "meshes/tibia_RM.mesh"
                            materials: principledMaterial
                        }
                    }
                }
            }
            Model {
                id: leg_RR_
                position: Qt.vector3d(67.3305, 6.21684, 129.574)
                source: "meshes/leg_RR.mesh"
                materials: principledMaterial
                Model {
                    id: coxa_RR_
                    position: Qt.vector3d(-8.26954, 11.5208, -31.4786)
                    scale: Qt.vector3d(1, 1, 1)
                    source: "meshes/coxa_RR.mesh"
                    materials: principledMaterial
                    Model {
                        id: femur_RR_
                        position: Qt.vector3d(54.1114, -3.07857, -32.92)
                        source: "meshes/femur_RR.mesh"
                        materials: principledMaterial
                        Model {
                            id: tibia_RR_
                            position: Qt.vector3d(91.5057, -0.350218, -0.110179)
                            source: "meshes/tibia_RR.mesh"
                            materials: principledMaterial
                        }
                    }
                }
            }
        }
        Model {
            id: frame1_006
            position: Qt.vector3d(-71.8552, 37.5384, 177.282)
            source: "meshes/frame1_006.mesh"
            materials: principledMaterial
        }
        Model {
            id: frame1_007
            position: Qt.vector3d(38.2976, 37.5384, 177.282)
            source: "meshes/frame1_007.mesh"
            materials: principledMaterial
        }
        Model {
            id: frame1
            position: Qt.vector3d(-71.8552, 36.827, 7.56421)
            source: "meshes/frame1.mesh"
            materials: principledMaterial
        }
        Model {
            id: frame1_001
            position: Qt.vector3d(37.0976, 34.7449, 7.47937)
            source: "meshes/frame1_001.mesh"
            materials: principledMaterial
        }
        Model {
            id: frame1_002
            position: Qt.vector3d(37.0976, -13.5332, 7.47937)
            source: "meshes/frame1_002.mesh"
            materials: principledMaterial
        }
        Model {
            id: frame1_003
            position: Qt.vector3d(-71.8552, -13.5332, 177.282)
            source: "meshes/frame1_003.mesh"
            materials: principledMaterial
        }
        Model {
            id: frame1_004
            position: Qt.vector3d(38.2976, -13.5332, 177.282)
            source: "meshes/frame1_004.mesh"
            materials: principledMaterial
        }
        Model {
            id: frame1_005
            position: Qt.vector3d(-71.8552, -13.5332, 7.56421)
            source: "meshes/frame1_005.mesh"
            materials: principledMaterial
        }
        Model {
            id: stick1_005
            position: Qt.vector3d(36.6469, 31.4347, -76.3885)
            source: "meshes/stick1_005.mesh"
            materials: principledMaterial
        }
        Model {
            id: stick1
            position: Qt.vector3d(36.6469, 29.7675, 6.28143)
            source: "meshes/stick1.mesh"
            materials: principledMaterial
        }
        Model {
            id: stick1_001
            position: Qt.vector3d(36.6469, 29.7675, 93.8915)
            source: "meshes/stick1_001.mesh"
            materials: principledMaterial
        }
        Model {
            id: stick1_002
            position: Qt.vector3d(36.6469, 29.7675, 177.763)
            source: "meshes/stick1_002.mesh"
            materials: principledMaterial
        }
        Model {
            id: stick1_003
            position: Qt.vector3d(36.6469, 29.7675, 264.919)
            source: "meshes/stick1_003.mesh"
            materials: principledMaterial
        }
        Model {
            id: stick1_004
            position: Qt.vector3d(36.6469, 29.7675, 347.298)
            source: "meshes/stick1_004.mesh"
            materials: principledMaterial
        }
    }

    // Animations:
    function setLegAngles(leg, coxa, femur, tibia){
        leg = leg - 1

        var coxaNodes = [coxa_LF_, coxa_LM_, coxa_LR_, coxa_RR_, coxa_RM_, coxa_RF_]
        var femurNodes = [femur_LF_, femur_LM_, femur_LR_, femur_RR_, femur_RM_, femur_RF_]
        var tibiaNodes = [tibia_LF_, tibia_LM_, tibia_LR_, tibia_RR_, tibia_RM_, tibia_RF_]

        if(leg < 0 || leg >= 6){
            return
        }
        else if(leg < 3){
            coxa += 180
        }

        // Blender -> QML:
        // x = x, y = z, z = -y
        coxaNodes[leg].eulerRotation.y = coxa
        femurNodes[leg].eulerRotation.z = -femur
        tibiaNodes[leg].eulerRotation.z = -tibia
    }
}
