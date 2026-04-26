#pragma once

#include <vector>
#include <string>
#include "Estructura.h"

struct ResultadoRuta {
    std::vector<Entrega> ruta;
    int costoTotal;
    bool rutaValida;
    std::string tecnica;
};

bool esTransitableRuta(int valor);

float calcularPesoTotalRuta(const std::vector<Entrega>& ruta);

bool validarPesoRuta(const std::vector<Entrega>& ruta, float capacidadMaxima);

int calcularDistanciaBFSRuta(
    const std::vector<std::vector<int>>& tablero,
    Punto origen,
    Punto destino
);

int calcularCostoRutaGeneral(
    const std::vector<std::vector<int>>& tablero,
    Punto inicio,
    const std::vector<Entrega>& ruta
);

void imprimirResultadoRuta(const ResultadoRuta& resultado);