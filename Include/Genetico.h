#pragma once

#include <vector>
#include "Estructura.h"
#include "RutasUtil.h"

ResultadoRuta algoritmoGenetico(
    const std::vector<std::vector<int>>& tablero,
    Punto inicio,
    std::vector<Entrega> entregas,
    float capacidadMaxima,
    int poblacionTam,
    int generaciones
);