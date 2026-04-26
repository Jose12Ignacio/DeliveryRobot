#pragma once

#include <vector>
#include "Estructura.h"
#include "RutasUtil.h"

struct ResultadoMochila {
    std::vector<Entrega> paquetesSeleccionados;
    float pesoTotal;
    int valorTotal;
    bool solucionValida;
};

ResultadoRuta algoritmoProgramacionDinamica(
    const std::vector<std::vector<int>>& tablero,
    Punto inicio,
    std::vector<Entrega> entregas,
    float capacidadMaxima
);

ResultadoMochila mochilaProgramacionDinamica(
    std::vector<Entrega> entregas,
    float capacidadMaxima
);

void imprimirResultadoMochila(const ResultadoMochila& resultado);