#pragma once

#include <vector>
#include "Estructura.h"
#include "RutasUtil.h"

ResultadoRuta algoritmoMonteCarlo(
    const std::vector<std::vector<int>>& tablero,
    Punto inicio,
    std::vector<Entrega> entregas,
    int iteraciones,
    float capacidadMaxima
);

ResultadoRuta algoritmoLasVegas(
    const std::vector<std::vector<int>>& tablero,
    Punto inicio,
    std::vector<Entrega> entregas,
    int maxIntentos,
    float capacidadMaxima
);