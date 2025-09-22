#ifndef __ANTONOV_AN225_HPP
#define __ANTONOV_AN225_HPP

#include "HEADERS//Orbitersdk.h"
#include "an225_definitions.h"

//Vessel const parameters
constexpr double AN225_SIZE = 25.0;  //Mean radius

constexpr double AN225_EMPTYMASS = 285000; //Empty mass in kg.

constexpr double AN225_FUELMASS = 300000; //Fuel mass in kg.

constexpr double AN225_ISP = 100000;  //Fuel-specific impulse in m/s.

constexpr double AN225_MAXMAINTH = 600e3; //Max main thrust in N.

constexpr double LANDING_GEAR_OPERATING_SPEED = 0.06;

constexpr double ENGINE_ROTATION_SPEED = 50;

constexpr VECTOR3 AN225_CS = {635.26, 1829.29, 283.37};

constexpr VECTOR3 AN225_PMI = {395.22, 482.37, 209.22};

constexpr double AN225_VLIFT_C = 10.28; //Chord lenght in meters;

constexpr double AN225_VLIFT_S = 905.0; //Wing area in m^2

constexpr double AN225_VLIFT_A = 0.1949;//Wing aspect ratio

constexpr double AN225_HLIFT_C = 2.27; //Vertical stabilizer chord lenght in meters

constexpr double AN225_HLIFT_S = 84.7755; //Vertical stabilizer area in m^2

constexpr double AN225_HLIFT_A = 0.1641; //Vertical stabilizer wing aspect ratio.

constexpr double AN225_HSTAB_C = 3.56; //Horizontal stabilizer chord lenght in meters

constexpr double AN225_HSTAB_S = 235.3179; //Horizontal stabilizer area in m^2

constexpr double AN225_HSTAB_A = 0.1020; //Horizontal stabilizer wing aspect ratio.

constexpr VECTOR3 AN225_COCKPIT_OFFSET = cockpit_windows_Location;

constexpr VECTOR3 FORWARD_DIRECTION = {0, 0, 1};

constexpr VECTOR3 BACKWARD_DIRECTION = {0, 0, -1};


//Define touchdown points
//For gear down
static const int ntdvtx_geardown = 13;
static TOUCHDOWNVTX tdvtx_geardown[ntdvtx_geardown] = {
    {(TDP1_Location), 5e6, 5e5, 1.6, 0.1},
    {(TDP2_Location), 5e6, 5e5, 3.0, 0.2},
    {(TDP3_Location), 5e6, 5e5, 3.0, 0.2},
    {(TDP4_Location), 5e6, 5e6, 3.0},
    {(TDP5_Location), 5e6, 5e6, 3.0},
    {(TDP6_Location), 5e6, 5e6, 3.0},
    {(TDP7_Location), 5e6, 5e6, 3.0},
    {(TDP8_Location), 5e6, 5e6, 3.0},
    {(TDP9_Location), 5e6, 5e6, 3.0},
    {(TDP10_Location), 5e6, 5e6, 3.0},
    {(TDP11_Location), 5e6, 5e6, 3.0},
    {(TDP12_Location), 5e6, 5e6, 3.0},
    {(TDP13_Location), 5e6, 5e6, 3.0},
};

static const int ntdvtx_gearup = 13;
static TOUCHDOWNVTX tdvtx_gearup[ntdvtx_geardown] = {
    {(TDP1_gear_up_Location), 5e6, 5e6, 3.0},
    {(TDP2_gear_up_Location), 5e6, 5e6, 3.0},
    {(TDP3_gear_up_Location), 5e6, 5e6, 3.0},
    {(TDP4_Location), 5e6, 5e6, 3.0},
    {(TDP5_Location), 5e6, 5e6, 3.0},
    {(TDP6_Location), 5e6, 5e6, 3.0},
    {(TDP7_Location), 5e6, 5e6, 3.0},
    {(TDP8_Location), 5e6, 5e6, 3.0},
    {(TDP9_Location), 5e6, 5e6, 3.0},
    {(TDP10_Location), 5e6, 5e6, 3.0},
    {(TDP11_Location), 5e6, 5e6, 3.0},
    {(TDP12_Location), 5e6, 5e6, 3.0},
    {(TDP13_Location), 5e6, 5e6, 3.0},
};


class AN225 : public VESSEL4{

    public:

        enum LandingGearStatus{GEAR_DOWN, GEAR_UP, GEAR_DEPLOYING, GEAR_STOWING} landing_gear_status;

        AN225(OBJHANDLE hVessel, int flightmodel);
        virtual ~AN225();

        void clbkSetClassCaps(FILEHANDLE cfg) override;
        void clbkPostStep(double simt, double simdt, double mjd) override;
        void clbkPreStep(double simt, double simdt, double mjd) override;
        int clbkConsumeBufferedKey(int key, bool down, char *kstate) override;

        void SetGearDown();
        void ActivateLandingGear(LandingGearStatus action);
        void UpdateLandingGearAnimation(double);
        void UpdateGearStatus(void);

        void ParkingBrake();

        void DefineAnimations();
        void DefineEngineAnimations();
        void DefineFrontGearAnimations();
        void DefineMainGearAnimations();
        void DefineControlSurfacesAnimations();

        void RunEnginesAnim(double simdt);
        double GetEnginePower();


        void EnginesAutoStartStop();
        void UpdateEnginesStatus();

        double UpdateLvlEnginesContrail();


        MESHHANDLE an225_mesh;
        unsigned int uimesh_an225;

        THRUSTER_HANDLE th_main[6], th_retro[6];
        THGROUP_HANDLE thg_main, thg_retro;

        AIRFOILHANDLE lwing, rwing;
        AIRFOILHANDLE lvstabilizer, rvstabilizer;
        AIRFOILHANDLE lhstabilizer, rhstabilizer;
        CTRLSURFHANDLE hlaileron, hraileron;

        unsigned int anim_laileron, anim_raileron;
        unsigned int anim_elevator, anim_elevator_trim;
        unsigned int anim_rudder;
        unsigned int anim_landing_gear;
        unsigned int anim_engines;

        double lvlcontrailengines;
        double landing_gear_proc;
        double engines_proc;
        
        bool bGearIsDown;
        bool engines_on;
        bool parkingBrakeEnabled;
        double pwrLvl;

};

#endif //!__ANTONOV_AN225_HPP