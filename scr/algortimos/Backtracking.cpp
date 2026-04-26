#include <vector>
#include <limits>

#include "../../Include/Backtracking.h"

using namespace std;

void backtrackingRecursivo(
    const vector<vector<int>>& tablero,
    Punto inicio,
    vector<Entrega>& entregas,
    vector<bool>& usado,
    vector<Entrega>& rutaActual,
    ResultadoRuta& mejorResultado
) {
    if (rutaActual.size() == entregas.size()) {
        int costo = calcularCostoRutaGeneral(tablero, inicio, rutaActual);

        if (costo != -1 && costo < mejorResultado.costoTotal) {
            mejorResultado.ruta = rutaActual;
            mejorResultado.costoTotal = costo;
            mejorResultado.rutaValida = true;
        }

        return;
    }

    for (int i = 0; i < (int)entregas.size(); i++) {
        if (!usado[i]) {
            usado[i] = true;
            rutaActual.push_back(entregas[i]);

            backtrackingRecursivo(
                tablero,
                inicio,
                entregas,
                usado,
                rutaActual,
                mejorResultado
            );

            rutaActual.pop_back();
            usado[i] = false;
        }
    }
}

ResultadoRuta algoritmoBacktracking(
    const vector<vector<int>>& tablero,
    Punto inicio,
    vector<Entrega> entregas,
    float capacidadMaxima
) {
    ResultadoRuta resultado;
    resultado.costoTotal = numeric_limits<int>::max();
    resultado.rutaValida = false;
    resultado.tecnica = "Backtracking";

    if (!validarPesoRuta(entregas, capacidadMaxima)) {
        return resultado;
    }

    vector<bool> usado(entregas.size(), false);
    vector<Entrega> rutaActual;

    backtrackingRecursivo(
        tablero,
        inicio,
        entregas,
        usado,
        rutaActual,
        resultado
    );

    return resultado;
}