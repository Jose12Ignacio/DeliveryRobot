#pragma once

#include <vector>
#include <string>
#include "Estructura.h"

struct ResultadoProbabilistico {
    std::vector<Entrega> ruta;
    int costoTotal;
    bool rutaValida;
    std::string tecnica;
    int intentosUsados;
};

ResultadoProbabilistico algoritmoMonteCarlo(
    const std::vector<std::vector<int>>& tablero,
    Punto inicio,
    std::vector<Entrega> entregas,
    int iteraciones,
    float capacidadMaxima
);

ResultadoProbabilistico algoritmoLasVegas(
    const std::vector<std::vector<int>>& tablero,
    Punto inicio,
    std::vector<Entrega> entregas,
    int maxIntentos,
    float capacidadMaxima
);

void imprimirResultadoProbabilistico(
    const ResultadoProbabilistico& resultado
);