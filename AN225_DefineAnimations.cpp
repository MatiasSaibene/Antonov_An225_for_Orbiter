#include "Antonov_AN225.hpp"

void AN225::DefineAnimations(){
    
    anim_landing_gear = CreateAnimation(0.0);
    DefineFrontGearAnimations();
    DefineMainGearAnimations();

    anim_laileron = CreateAnimation(0.5);
    anim_raileron = CreateAnimation(0.5);
    anim_elevator = CreateAnimation(0.5);
    anim_elevator_trim = CreateAnimation(0.5);
    anim_rudder = CreateAnimation(0.5);
    DefineControlSurfacesAnimations();

    anim_engines = CreateAnimation(0.0);
    DefineEngineAnimations();

};


void AN225::DefineFrontGearAnimations(){

    //Front left gear landing left gear door
    static unsigned int FrontLeftLandingGearLeftDoorGrp[1] = {front_left_gear_left_door_Id};
    static MGROUP_ROTATE FrontLeftLandingGearLeftDoor(
        0,
        FrontLeftLandingGearLeftDoorGrp,
        1,
        (Axis_front_left_gear_left_door_Location),
        _V(0, 0, 1),
        (float)(100*RAD)
    );

    //Front left gear landing right gear door
    static unsigned int FrontLeftLandingGearRightDoorGrp[1] = {front_left_gear_right_door_Id};
    static MGROUP_ROTATE FrontLeftLandingGearRightDoor(
        0,
        FrontLeftLandingGearRightDoorGrp,
        1,
        (Axis_front_left_gear_right_door_Location),
        _V(0, 0, 1),
        (float)(-90*RAD)
    );

    //Front left landing gear
    static unsigned int FrontLeftLandingGearGrp[1] = {front_landing_gear_left_wheels_Id};
    static MGROUP_ROTATE FrontLeftLandingGear(
        0,
        FrontLeftLandingGearGrp,
        1,
        (Axis_front_left_gear_wheels_Location),
        _V(1, 0, 0),
        (float)(110*RAD)
    );


    //Front right gear landing left gear door
    static unsigned int FrontRightLandingGearLeftDoorGrp[1] = {front_right_gear_left_door_Id};
    static MGROUP_ROTATE FrontRightLandingGearLeftDoor(
        0,
        FrontRightLandingGearLeftDoorGrp,
        1,
        (Axis_front_right_gear_left_door_Location),
        _V(0, 0, 1),
        (float)(90*RAD)
    );

    //Front right gear landing right gear door
    static unsigned int FrontRightLandingGearRightDoorGrp[1] = {front_right_gear_right_door_Id};
    static MGROUP_ROTATE FrontRightLandingGearRightDoor(
        0,
        FrontRightLandingGearRightDoorGrp,
        1,
        (Axis_front_right_gear_right_door_Location),
        _V(0, 0, 1),
        (float)(-100*RAD)
    );


    //Front right landing gear
    static unsigned int FrontRightLandingGearGrp[1] = {front_landing_gear_right_wheels_Id};
    static MGROUP_ROTATE FrontRightLandingGear(
        0,
        FrontRightLandingGearGrp,
        1,
        (Axis_front_right_gear_wheels_Location),
        _V(1, 0, 0),
        (float)(110*RAD)
    );

    AddAnimationComponent(anim_landing_gear, 0.25, 1, &FrontLeftLandingGearLeftDoor);
    AddAnimationComponent(anim_landing_gear, 0.25, 1, &FrontLeftLandingGearRightDoor);
    AddAnimationComponent(anim_landing_gear, 0.25, 1, &FrontRightLandingGearLeftDoor);
    AddAnimationComponent(anim_landing_gear, 0.25, 1, &FrontRightLandingGearRightDoor);

    AddAnimationComponent(anim_landing_gear, 0, 0.75, &FrontLeftLandingGear);
    AddAnimationComponent(anim_landing_gear, 0, 0.75, &FrontRightLandingGear);

}

void AN225::DefineMainGearAnimations(){

    static unsigned int LowerMainGearLeftDoorGrp[1] = {left_lower_main_gear_door_Id};
    static MGROUP_ROTATE LowerMainGearLeftDoorRot1(
        0,
        LowerMainGearLeftDoorGrp,
        1,
        (Axis_Left_gear_doors_rotation_lower_45d_Location),
        _V(0, 0, 1),
        (float)(45*RAD)
    );

    static unsigned int MainGearLeftWheelsGrp[7] = {gear_left_wheel_01_Id, gear_left_wheel_02_Id, gear_left_wheel_03_Id, gear_left_wheel_04_Id, gear_left_wheel_05_Id, gear_left_wheel_06_Id, gear_left_wheel_07_Id};
    static MGROUP_ROTATE MainGearLeftWheels(
        0,
        MainGearLeftWheelsGrp,
        7,
        (Axis_left_main_gear_wheels_Location),
        _V(0, 0, 1),
        (float)(110*RAD)
    );

    static MGROUP_ROTATE LowerMainGearLeftDoorRot2(
        0,
        LowerMainGearLeftDoorGrp,
        1,
        (Axis_Left_gear_doors_rotation_lower_45d_Location),
        _V(0, 0, 1),
        (float)(-45*RAD)
    );

    static unsigned int UpperMainGearLeftDoorGrp[1] = {left_upper_main_gear_door_Id};
    static MGROUP_ROTATE UpperMainGearLeftDoor(
        0,
        UpperMainGearLeftDoorGrp,
        1,
        (Axis_Left_gear_doors_rotation_upper_60d_Location),
        _V(0, 0, 1),
        (float)(60*RAD)
    );

    AddAnimationComponent(anim_landing_gear, 0, 0.25, &LowerMainGearLeftDoorRot1);
    AddAnimationComponent(anim_landing_gear, 0.25, 0.75, &MainGearLeftWheels);
    AddAnimationComponent(anim_landing_gear, 0.75, 1, &LowerMainGearLeftDoorRot2);
    AddAnimationComponent(anim_landing_gear, 0.75, 1, &UpperMainGearLeftDoor);



    static unsigned int LowerMainGearRightDoorGrp[1] = {right_lower_main_gear_door_Id};
    static MGROUP_ROTATE LowerMainGearRightDoorRot1(
        0,
        LowerMainGearRightDoorGrp,
        1,
        (Axis_Right_gear_doors_rotation_lower_45d_Location),
        _V(0, 0, 1),
        (float)(-45*RAD)
    );

    static unsigned int MainGearRightWheelsGrp[7] = {gear_right_wheel_01_Id, gear_right_wheel_02_Id, gear_right_wheel_03_Id, gear_right_wheel_04_Id, gear_right_wheel_05_Id, gear_right_wheel_06_Id, gear_right_wheel_07_Id};
    static MGROUP_ROTATE MainGearRightWheels(
        0,
        MainGearRightWheelsGrp,
        7,
        (Axis_right_main_gear_wheels_Location),
        _V(0, 0, 1),
        (float)(-110*RAD)
    );

    static MGROUP_ROTATE LowerMainGearRightDoorRot2(
        0,
        LowerMainGearRightDoorGrp,
        1,
        (Axis_Right_gear_doors_rotation_lower_45d_Location),
        _V(0, 0, 1),
        (float)(45*RAD)
    );

    static unsigned int UpperMainGearRightDoorGrp[1] = {right_upper__main_gear_door_Id};
    static MGROUP_ROTATE UpperMainGearRightDoor(
        0,
        UpperMainGearRightDoorGrp,
        1,
        (Axis_Right_gear_doors_rotation_upper_60d_Location),
        _V(0, 0, 1),
        (float)(-60*RAD)
    );

    AddAnimationComponent(anim_landing_gear, 0, 0.25, &LowerMainGearRightDoorRot1);
    AddAnimationComponent(anim_landing_gear, 0.25, 0.75, &MainGearRightWheels);
    AddAnimationComponent(anim_landing_gear, 0.75, 1, &LowerMainGearRightDoorRot2);
    AddAnimationComponent(anim_landing_gear, 0.75, 1, &UpperMainGearRightDoor);
    
}

void AN225::DefineControlSurfacesAnimations(){

    static unsigned int LeftMainWingElevatorGrp[1] = {main_wing_elevator_left_Id};
    static MGROUP_ROTATE LeftMainWingElevator(
        0,
        LeftMainWingElevatorGrp,
        1,
        (Axis_Left_main_wing_elevator_Location),
        _V(1, 0, 0),
        (float)(35*RAD)
    );

    static unsigned int RightMainWingElevatorGrp[1] = {main_wing_elevator_right_Id};
    static MGROUP_ROTATE RightMainWingElevator(
        0,
        RightMainWingElevatorGrp,
        1,
        (Axis_Right_main_wing_elevator_Location),
        _V(1, 0, 0),
        (float)(35*RAD)
    );

    AddAnimationComponent(anim_elevator, 0, 1, &LeftMainWingElevator);
    AddAnimationComponent(anim_elevator, 0, 1, &RightMainWingElevator);


    static unsigned int LeftElevatorGrp[1] = {left_elevator_Id};
    static MGROUP_ROTATE LeftElevator(
        0,
        LeftElevatorGrp,
        1,
        (Axis_Left_elevator_Location),
        _V(1, 0, 0.45),
        (float)(35*RAD)
    );

    static unsigned int RightElevatorGrp[1] = {right_elevator_Id};
    static MGROUP_ROTATE RightElevator(
        0,
        RightElevatorGrp,
        1,
        (Axis_Right_elevator_Location),
        _V(1, 0, -0.45),
        (float)(35*RAD)
    );

    AddAnimationComponent(anim_elevator, 0, 1, &LeftElevator);
    AddAnimationComponent(anim_elevator, 0, 1, &RightElevator);


    static unsigned int LeftElevatorTrimGrp[1] = {left_elevator_Id};
    static MGROUP_ROTATE LeftElevatorTrim(
        0,
        LeftElevatorGrp,
        1,
        (Axis_Left_elevator_Location),
        _V(1, 0, 0.45),
        (float)(17*RAD)
    );

    static unsigned int RightElevatorTrimGrp[1] = {right_elevator_Id};
    static MGROUP_ROTATE RightElevatorTrim(
        0,
        RightElevatorGrp,
        1,
        (Axis_Right_elevator_Location),
        _V(1, 0, -0.45),
        (float)(17*RAD)
    );

    AddAnimationComponent(anim_elevator_trim, 0, 1, &LeftElevatorTrim);
    AddAnimationComponent(anim_elevator_trim, 0, 1, &RightElevatorTrim);


    static unsigned int LeftRudderGrp[1] = {left_rudder_Id};
    static MGROUP_ROTATE LeftRudder(
        0,
        LeftRudderGrp,
        1,
        (Axis_Left_rudder_Location),
        _V(0, 1, 0),
        (float)(35*RAD)
    );

    static unsigned int RightRudderGrp[1] = {right_rudder_Id};
    static MGROUP_ROTATE RightRudder(
        0,
        RightRudderGrp,
        1,
        (Axis_Right_rudder_Location),
        _V(0, 1, 0),
        (float)(35*RAD)
    );

    AddAnimationComponent(anim_rudder, 0, 1, &LeftRudder);
    AddAnimationComponent(anim_rudder, 0, 1, &RightRudder);

    static unsigned int LeftAileronGrp[1] = {left_aileron_Id};
    static MGROUP_ROTATE LeftAileron(
        0,
        LeftAileronGrp,
        1,
        (Axis_Left_aileron_Location),
        _V(1, 0, 0.45),
        (float)(35*RAD)
    );

    static unsigned int RightAileronGrp[1] = {right_aileron_Id};
    static MGROUP_ROTATE RightAileron(
        0,
        RightAileronGrp,
        1,
        (Axis_Right_aileron_Location),
        _V(1, 0, -0.45),
        (float)(35*RAD)
    );

    AddAnimationComponent(anim_laileron, 0, 1, &LeftAileron);
    AddAnimationComponent(anim_raileron, 0, 1, &RightAileron);
}

void AN225::DefineEngineAnimations(){

    static unsigned int Engine1Grp[1] = {ENG1_blades_Id};
    static MGROUP_ROTATE Engine1(
        0,
        Engine1Grp,
        1,
        (ENG1_blades_Location),
        _V(0, 0, 1),
        (float)(2 * PI * 20)
    );

    static unsigned int Engine2Grp[1] = {ENG2_blades_Id};
    static MGROUP_ROTATE Engine2(
        0,
        Engine2Grp,
        1,
        (ENG2_blades_Location),
        _V(0, 0, 1),
        (float)(2 * PI * 20)
    );

    static unsigned int Engine3Grp[1] = {ENG3_blades_Id};
    static MGROUP_ROTATE Engine3(
        0,
        Engine3Grp,
        1,
        (ENG3_blades_Location),
        _V(0, 0, 1),
        (float)(2 * PI * 20)
    );

    static unsigned int Engine4Grp[1] = {ENG4_blades_Id};
    static MGROUP_ROTATE Engine4(
        0,
        Engine4Grp,
        1,
        (ENG4_blades_Location),
        _V(0, 0, 1),
        (float)(2 * PI * 20)
    );

    static unsigned int Engine5Grp[1] = {ENG5_blades_Id};
    static MGROUP_ROTATE Engine5(
        0,
        Engine5Grp,
        1,
        (ENG5_blades_Location),
        _V(0, 0, 1),
        (float)(2 * PI * 20)
    );

    static unsigned int Engine6Grp[1] = {ENG6_blades_Id};
    static MGROUP_ROTATE Engine6(
        0,
        Engine6Grp,
        1,
        (ENG6_blades_Location),
        _V(0, 0, 1),
        (float)(2 * PI * 20)
    );

    AddAnimationComponent(anim_engines, 0, 1, &Engine1);
    AddAnimationComponent(anim_engines, 0, 1, &Engine2);
    AddAnimationComponent(anim_engines, 0, 1, &Engine3);
    AddAnimationComponent(anim_engines, 0, 1, &Engine4);
    AddAnimationComponent(anim_engines, 0, 1, &Engine5);
    AddAnimationComponent(anim_engines, 0, 1, &Engine6);
}