#include "Antonov_AN225.hpp"
#include <cmath>

void AN225::RunEnginesAnim(double simdt){

    double pwr = GetEnginePower(); // 0.0 - 1.0

    // velocidad mínima (windmilling) cuando motor está apagado
    const double min_speed = 0.05; // ajustá este valor a gusto
    const double max_speed = ENGINE_ROTATION_SPEED;

    // velocidad efectiva = mínima + (máxima - mínima) * potencia
    double spin_speed = min_speed + (max_speed - min_speed) * pwr;

    // avanzar el acumulador
    engines_proc += simdt * spin_speed;

    // normalizar a [0,1]
    if (engines_proc > 1.0)
        engines_proc -= floor(engines_proc);

    SetAnimation(anim_engines, engines_proc);

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

void AN225::UpdateExitDoorAnimation(double simdt) {
    if (exitDoor_status >= DOOR_CLOSING) {
        double da = simdt * LANDING_GEAR_OPERATING_SPEED;
        if (exitDoor_status == DOOR_CLOSING) {
            if (exitDoor_proc > 0.0) exitDoor_proc = std::max(0.0, exitDoor_proc - da);
            else exitDoor_status = DOOR_CLOSED;
        } else {
            if (exitDoor_proc < 1.0) exitDoor_proc = std::min(1.0, exitDoor_proc + da);
            else exitDoor_status = DOOR_OPEN;
        }
        SetAnimation(anim_exitDoor, exitDoor_proc);
    }
}