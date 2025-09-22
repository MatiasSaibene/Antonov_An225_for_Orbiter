//Copyright (c) Matías Saibene
//Licenced under the MIT Licence

//==========================================
//          ORBITER MODULE: Antonov AN225
//
//Antonov_AN225.cpp
//Control module for Antonov AN225 vessel class
//
//==========================================

#define ORBITER_MODULE
#include "Antonov_AN225.hpp"

// 1. vertical lift component

void VLiftCoeff (VESSEL *v, double aoa, double M, double Re, void *context, double *cl, double *cm, double *cd)
{
	const int nabsc = 9;
	static const double AOA[nabsc] = {-180*RAD,-60*RAD,-30*RAD, -15*RAD, 0*RAD,15*RAD,30*RAD,60*RAD,180*RAD};
	static const double CL[nabsc]  = {   0,    -0.56,   -0.56,   -0.16,  0.15,  0.46,  0.56,  0.56,  0.00};
	static const double CM[nabsc]  = {    0,    0.00,   0.00,     0.00,  0.00,  0.00,  0.00,  0.00,  0.00};


	int i;
	for (i = 0; i < nabsc-1 && AOA[i+1] < aoa; i++);
	if (i < nabsc - 1) {
		double f = (aoa - AOA[i]) / (AOA[i + 1] - AOA[i]);
		*cl = CL[i] + (CL[i + 1] - CL[i]) * f;  // aoa-dependent lift coefficient
		*cm = CM[i] + (CM[i + 1] - CM[i]) * f;  // aoa-dependent moment coefficient
	}
	else {
		*cl = CL[nabsc - 1];
		*cm = CM[nabsc - 1];
	}
	double saoa = sin(aoa);
	double pd = 0.015 + 0.4*saoa*saoa;  // profile drag
	*cd = pd + oapiGetInducedDrag (*cl, AN225_VLIFT_A, 0.7) + oapiGetWaveDrag (M, 0.75, 1.0, 1.1, 0.04);
	// profile drag + (lift-)induced drag + transonic/supersonic wave (compressibility) drag
}

// 2. horizontal lift component (vertical stabilisers and body)

void HLiftCoeff (VESSEL *v, double beta, double M, double Re, void *context, double *cl, double *cm, double *cd)
{
	int i;
	const int nabsc = 8;
	static const double BETA[nabsc] = {-180*RAD,-135*RAD,-90*RAD,-45*RAD,45*RAD,90*RAD,135*RAD,180*RAD};
	static const double CL[nabsc]   = {       0,    +0.3,      0,   -0.3,  +0.3,     0,   -0.3,      0};
	for (i = 0; i < nabsc-1 && BETA[i+1] < beta; i++);
	if (i < nabsc - 1) {
		*cl = CL[i] + (CL[i + 1] - CL[i]) * (beta - BETA[i]) / (BETA[i + 1] - BETA[i]);
	}
	else {
		*cl = CL[nabsc - 1];
	}
	*cm = 0.0;
	*cd = 0.015 + oapiGetInducedDrag (*cl, AN225_HLIFT_A, 0.6) + oapiGetWaveDrag (M, 0.75, 1.0, 1.1, 0.04);
}

AN225::AN225(OBJHANDLE hVessel, int flightmodel) : VESSEL4(hVessel, flightmodel){

    an225_mesh = nullptr;

    uimesh_an225 = 0;

    for(unsigned int i = 0; i < 6; i++){
        th_main[i] = nullptr;
    }

    for(unsigned int i = 0; i < 6; i++){
        th_retro[i] = nullptr;
    }

    thg_main = nullptr;

    thg_retro = nullptr;

    lwing = nullptr;

    rwing = nullptr;

    lvstabilizer = nullptr;

    rvstabilizer = nullptr;

    lhstabilizer = nullptr;

    rhstabilizer = nullptr;

    hlaileron = nullptr;

    hraileron = nullptr;

    anim_laileron = 0;

    anim_raileron = 0;

    anim_elevator = 0;

    anim_rudder = 0;
    
    lvlcontrailengines = 0.0;

    bGearIsDown = true;

    engines_on = false;

    landing_gear_proc = 0.0;

    DefineAnimations();

}

AN225::~AN225(){

}

void AN225::clbkSetClassCaps(FILEHANDLE cfg){

    SetSize(AN225_SIZE);
    SetEmptyMass(AN225_EMPTYMASS);
    SetCrossSections(AN225_CS);
    SetPMI(AN225_PMI);
    SetMaxWheelbrakeForce(89e3);
    SetRotDrag(_V(12, 12, 2.5));
    SetCameraOffset(AN225_COCKPIT_OFFSET);

    PROPELLANT_HANDLE JET_A1 = CreatePropellantResource(AN225_FUELMASS);

    th_main[0] = CreateThruster((ENG1_Location), FORWARD_DIRECTION, AN225_MAXMAINTH, JET_A1, AN225_ISP);
    th_main[1] = CreateThruster((ENG2_Location), FORWARD_DIRECTION, AN225_MAXMAINTH, JET_A1, AN225_ISP);
    th_main[2] = CreateThruster((ENG3_Location), FORWARD_DIRECTION, AN225_MAXMAINTH, JET_A1, AN225_ISP);
    th_main[3] = CreateThruster((ENG4_Location), FORWARD_DIRECTION, AN225_MAXMAINTH, JET_A1, AN225_ISP);
    th_main[4] = CreateThruster((ENG5_Location), FORWARD_DIRECTION, AN225_MAXMAINTH, JET_A1, AN225_ISP);
    th_main[5] = CreateThruster((ENG6_Location), FORWARD_DIRECTION, AN225_MAXMAINTH, JET_A1, AN225_ISP);

    th_retro[0] = CreateThruster((ENG1_Location), BACKWARD_DIRECTION, (AN225_MAXMAINTH/4), JET_A1, AN225_ISP);
    th_retro[1] = CreateThruster((ENG2_Location), BACKWARD_DIRECTION, (AN225_MAXMAINTH/4), JET_A1, AN225_ISP);
    th_retro[2] = CreateThruster((ENG3_Location), BACKWARD_DIRECTION, (AN225_MAXMAINTH/4), JET_A1, AN225_ISP);
    th_retro[3] = CreateThruster((ENG4_Location), BACKWARD_DIRECTION, (AN225_MAXMAINTH/4), JET_A1, AN225_ISP);
    th_retro[4] = CreateThruster((ENG5_Location), BACKWARD_DIRECTION, (AN225_MAXMAINTH/4), JET_A1, AN225_ISP);
    th_retro[5] = CreateThruster((ENG6_Location), BACKWARD_DIRECTION, (AN225_MAXMAINTH/4), JET_A1, AN225_ISP);

    //Contrail effect on engines
    static PARTICLESTREAMSPEC engines_contrails = {
        0, 0.5, .95, 120, 0.03, 10.0, 5, 3.0, 
        PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_PLIN, -1.0, 25.0,
		PARTICLESTREAMSPEC::ATM_PLIN, 
    };

    AddParticleStream(&engines_contrails, ENG1_Location, BACKWARD_DIRECTION, &lvlcontrailengines);

    AddParticleStream(&engines_contrails, ENG2_Location, BACKWARD_DIRECTION, &lvlcontrailengines);

    AddParticleStream(&engines_contrails, ENG3_Location, BACKWARD_DIRECTION, &lvlcontrailengines);

    AddParticleStream(&engines_contrails, ENG4_Location, BACKWARD_DIRECTION, &lvlcontrailengines);

    AddParticleStream(&engines_contrails, ENG5_Location, BACKWARD_DIRECTION, &lvlcontrailengines);

    AddParticleStream(&engines_contrails, ENG6_Location, BACKWARD_DIRECTION, &lvlcontrailengines);


    lwing = CreateAirfoil3(LIFT_VERTICAL, left_main_wing_Location, VLiftCoeff, 0, AN225_VLIFT_C, AN225_VLIFT_S, AN225_VLIFT_A);
    rwing = CreateAirfoil3(LIFT_VERTICAL, right_main_wing_Location, VLiftCoeff, 0, AN225_VLIFT_C, AN225_VLIFT_S, AN225_VLIFT_A);

    lvstabilizer = CreateAirfoil3(LIFT_HORIZONTAL, (left_vertical_stab_Location), HLiftCoeff, 0, AN225_HLIFT_C, AN225_HLIFT_S, AN225_HLIFT_A);
    rvstabilizer = CreateAirfoil3(LIFT_HORIZONTAL, (right_vertical_stab_Location), HLiftCoeff, 0, AN225_HLIFT_C, AN225_HLIFT_S, AN225_HLIFT_A);

    lhstabilizer = CreateAirfoil3(LIFT_VERTICAL, (left_horizontal_stabilizer_Location), VLiftCoeff, 0, AN225_HSTAB_C, AN225_HSTAB_S, AN225_HSTAB_A);
    rhstabilizer = CreateAirfoil3(LIFT_VERTICAL, (right_horizontal_stabilizer_Location), VLiftCoeff, 0, AN225_HSTAB_C, AN225_HSTAB_S, AN225_HSTAB_A);

    hlaileron = CreateControlSurface3(AIRCTRL_AILERON, 47.5263, 1.7, left_aileron_Location, AIRCTRL_AXIS_AUTO, 1.0, anim_laileron);
    hraileron = CreateControlSurface3(AIRCTRL_AILERON, 47.5263, 1.7, right_aileron_Location, AIRCTRL_AXIS_AUTO, 1.0, anim_raileron);

    CreateControlSurface3(AIRCTRL_ELEVATOR, 34.2099, 1.7, left_elevator_Location, AIRCTRL_AXIS_AUTO, 1.0, anim_elevator);
    CreateControlSurface3(AIRCTRL_ELEVATOR, 34.2099, 1.7, right_elevator_Location, AIRCTRL_AXIS_AUTO, 1.0, anim_elevator);

    CreateControlSurface3(AIRCTRL_ELEVATORTRIM, 34.2099, 1.7, left_elevator_Location, AIRCTRL_AXIS_AUTO, 1.0, anim_elevator_trim);
    CreateControlSurface3(AIRCTRL_ELEVATORTRIM, 34.2099, 1.7, right_elevator_Location, AIRCTRL_AXIS_AUTO, 1.0, anim_elevator_trim);


    CreateControlSurface3(AIRCTRL_RUDDER, 18.3354, 1.7, left_rudder_Location, AIRCTRL_AXIS_AUTO, 1.0, anim_rudder);
    CreateControlSurface3(AIRCTRL_RUDDER, 18.3354, 1.7, right_rudder_Location, AIRCTRL_AXIS_AUTO, 1.0, anim_rudder);

    //Add the mesh
    SetMeshVisibilityMode(AddMesh(an225_mesh = oapiLoadMeshGlobal("Antonov_AN225\\AN225")), MESHVIS_EXTERNAL);

    SetTouchdownPoints(tdvtx_geardown, ntdvtx_geardown);
}

void AN225::ParkingBrake(){

    if(!parkingBrakeEnabled){
        SetWheelbrakeLevel(1, 0, true);
        parkingBrakeEnabled = true;
    } else {
        SetWheelbrakeLevel(0, 0, true);
        parkingBrakeEnabled = false;
    }

}

int AN225::clbkConsumeBufferedKey(int key, bool down, char *kstate){

    if(key == OAPI_KEY_G && down){
        SetGearDown();
        return 1;
    }

    if(down){
        if(KEYMOD_CONTROL(kstate)){
            switch(key){
                case OAPI_KEY_A:
                UpdateEnginesStatus();
                return 1;
            }
        }
    }

    if(key == OAPI_KEY_NUMPADENTER && down){
        ParkingBrake();
        return 1;
    }

    return 0;
}

void AN225::clbkPreStep(double simt, double simdt, double mjd){
}

void AN225::clbkPostStep(double simt, double simdt, double mjd){
    UpdateLandingGearAnimation(simdt);
    lvlcontrailengines = UpdateLvlEnginesContrail();
    
    if(engines_on && thg_main != nullptr){
        RunEnginesAnim(simdt);
    }
}

void AN225::UpdateEnginesStatus(){

    if(!engines_on){
        engines_on = true;
    } else if(engines_on == true){
        engines_on = false;
    }

    EnginesAutoStartStop();

}

void AN225::EnginesAutoStartStop(){

    if(engines_on == true){
        thg_main = CreateThrusterGroup(th_main, 6, THGROUP_MAIN);
        thg_retro = CreateThrusterGroup(th_retro, 6, THGROUP_RETRO);
    } else if(engines_on == false){
        DelThrusterGroup(thg_main);
        DelThrusterGroup(thg_retro);
    }

}

double AN225::GetEnginePower() {
    if (!engines_on){
        return 0.0;
    }
    return GetThrusterLevel(thg_main);
}

double AN225::UpdateLvlEnginesContrail(){
    double machnumber = GetMachNumber();
    double altitude = GetAltitude();

    if((machnumber > 0.5) && ((altitude > 10000) && (altitude < 15000))){
        return 1.0;
    } else {
        return 0.0;
    }

}

void AN225::SetGearDown(void){
    ActivateLandingGear((landing_gear_status == GEAR_DOWN || landing_gear_status == GEAR_DEPLOYING) ?
        GEAR_STOWING : GEAR_DEPLOYING);
}

void AN225::ActivateLandingGear(LandingGearStatus action){
    landing_gear_status = action;
}

void AN225::UpdateLandingGearAnimation(double simdt) {
    if (landing_gear_status >= GEAR_DEPLOYING) {
        double da = simdt * LANDING_GEAR_OPERATING_SPEED;
        if (landing_gear_status == GEAR_DEPLOYING) {
            if (landing_gear_proc > 0.0) landing_gear_proc = std::max(0.0, landing_gear_proc - da);
            else landing_gear_status = GEAR_DOWN;
            SetTouchdownPoints(tdvtx_geardown, ntdvtx_geardown);
            bGearIsDown = true;
        } else {
            if (landing_gear_proc < 1.0) landing_gear_proc = std::min(1.0, landing_gear_proc + da);
            else landing_gear_status = GEAR_UP;
            SetTouchdownPoints(tdvtx_gearup, ntdvtx_gearup);
            bGearIsDown = false;
        }
        SetAnimation(anim_landing_gear, landing_gear_proc);
    }
}

void AN225::UpdateGearStatus(void){
    if(bGearIsDown){
        SetTouchdownPoints(tdvtx_geardown, ntdvtx_geardown);
        SetNosewheelSteering(true);
    } else if (!bGearIsDown){
        SetTouchdownPoints(tdvtx_gearup, ntdvtx_gearup);
        SetNosewheelSteering(false);
    }
}

////////////////////////

DLLCLBK void InitModule(MODULEHANDLE hModule){


}

DLLCLBK void ExitModule(MODULEHANDLE *hModule){

}



///////////////Vessel initialization

DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel, int flightmodel){
    
	return new AN225(hvessel, flightmodel);

}

/////////////Vessel memory cleanup
DLLCLBK void ovcExit(VESSEL *vessel){
    
	if(vessel) delete(AN225*)vessel;
	
}