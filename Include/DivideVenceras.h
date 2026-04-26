#pragma once

#include <vector>
#include "Estructura.h"

ResultadoDV resolverCuadrante(
    const std::vector<std::vector<int>>& tablero,
    int fi,
    int ff,
    int ci,
    int cf
);

void imprimirResultadoDV(const ResultadoDV& r);