#pragma once

#include <vector>
#include "Estructura.h"
#include "RutasUtil.h"

ResultadoRuta algoritmoGreedy(
    const std::vector<std::vector<int>>& tablero,
    Punto inicio,
    std::vector<Entrega> entregas,
    float capacidadMaxima
);