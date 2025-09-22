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