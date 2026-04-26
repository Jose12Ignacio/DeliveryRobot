#include <vector>
#include <algorithm>
#include <random>
#include <limits>

#include "../../Include/Probabilisticos.h"

using namespace std;

ResultadoRuta algoritmoMonteCarlo(
    const vector<vector<int>>& tablero,
    Punto inicio,
    vector<Entrega> entregas,
    int iteraciones,
    float capacidadMaxima
) {
    ResultadoRuta mejorResultado;

    mejorResultado.costoTotal = numeric_limits<int>::max();
    mejorResultado.rutaValida = false;
    mejorResultado.tecnica = "Monte Carlo";

    random_device rd;
    mt19937 generador(rd());

    for (int i = 0; i < iteraciones; i++) {
        vector<Entrega> rutaAleatoria = entregas;

        shuffle(rutaAleatoria.begin(), rutaAleatoria.end(), generador);

        if (!validarPesoRuta(rutaAleatoria, capacidadMaxima)) {
            continue;
        }

        int costo = calcularCostoRutaGeneral(tablero, inicio, rutaAleatoria);

        if (costo != -1 && costo < mejorResultado.costoTotal) {
            mejorResultado.ruta = rutaAleatoria;
            mejorResultado.costoTotal = costo;
            mejorResultado.rutaValida = true;
        }
    }

    return mejorResultado;
}

ResultadoRuta algoritmoLasVegas(
    const vector<vector<int>>& tablero,
    Punto inicio,
    vector<Entrega> entregas,
    int maxIntentos,
    float capacidadMaxima
) {
    ResultadoRuta resultado;

    resultado.costoTotal = numeric_limits<int>::max();
    resultado.rutaValida = false;
    resultado.tecnica = "Las Vegas";

    random_device rd;
    mt19937 generador(rd());

    for (int intento = 1; intento <= maxIntentos; intento++) {
        vector<Entrega> rutaAleatoria = entregas;

        shuffle(rutaAleatoria.begin(), rutaAleatoria.end(), generador);

        if (!validarPesoRuta(rutaAleatoria, capacidadMaxima)) {
            continue;
        }

        int costo = calcularCostoRutaGeneral(tablero, inicio, rutaAleatoria);

        if (costo != -1) {
            resultado.ruta = rutaAleatoria;
            resultado.costoTotal = costo;
            resultado.rutaValida = true;
            return resultado;
        }
    }

    return resultado;
}