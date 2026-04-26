#include <iostream>
#include <vector>
#include <queue>
#include <limits>

#include "../../Include/RutasUtil.h"

using namespace std;

bool esTransitableRuta(int valor) {
    return valor == 1 || valor == 2 || valor == 3;
}

float calcularPesoTotalRuta(const vector<Entrega>& ruta) {
    float pesoTotal = 0.0f;

    for (const Entrega& entrega : ruta) {
        pesoTotal += entrega.peso;
    }

    return pesoTotal;
}

bool validarPesoRuta(const vector<Entrega>& ruta, float capacidadMaxima) {
    return calcularPesoTotalRuta(ruta) <= capacidadMaxima;
}

int calcularDistanciaBFSRuta(
    const vector<vector<int>>& tablero,
    Punto origen,
    Punto destino
) {
    if (tablero.empty()) {
        return -1;
    }

    int filas = tablero.size();
    int columnas = tablero[0].size();

    if (origen.fila < 0 || origen.fila >= filas ||
        origen.columna < 0 || origen.columna >= columnas ||
        destino.fila < 0 || destino.fila >= filas ||
        destino.columna < 0 || destino.columna >= columnas) {
        return -1;
    }

    vector<vector<bool>> visitado(filas, vector<bool>(columnas, false));
    vector<vector<int>> distancia(filas, vector<int>(columnas, 0));

    queue<Punto> cola;
    cola.push(origen);
    visitado[origen.fila][origen.columna] = true;

    int df[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    while (!cola.empty()) {
        Punto actual = cola.front();
        cola.pop();

        if (actual.fila == destino.fila &&
            actual.columna == destino.columna) {
            return distancia[actual.fila][actual.columna];
        }

        for (int i = 0; i < 4; i++) {
            int nf = actual.fila + df[i];
            int nc = actual.columna + dc[i];

            if (nf >= 0 && nf < filas &&
                nc >= 0 && nc < columnas &&
                !visitado[nf][nc] &&
                esTransitableRuta(tablero[nf][nc])) {

                visitado[nf][nc] = true;
                distancia[nf][nc] =
                    distancia[actual.fila][actual.columna] + 1;

                cola.push({nf, nc});
            }
        }
    }

    return -1;
}

int calcularCostoRutaGeneral(
    const vector<vector<int>>& tablero,
    Punto inicio,
    const vector<Entrega>& ruta
) {
    if (ruta.empty()) {
        return 0;
    }

    int costoTotal = 0;
    Punto actual = inicio;

    for (const Entrega& entrega : ruta) {
        int distancia = calcularDistanciaBFSRuta(
            tablero,
            actual,
            entrega.destino
        );

        if (distancia == -1) {
            return -1;
        }

        costoTotal += distancia;
        actual = entrega.destino;
    }

    int regreso = calcularDistanciaBFSRuta(tablero, actual, inicio);

    if (regreso == -1) {
        return -1;
    }

    costoTotal += regreso;

    return costoTotal;
}

void imprimirResultadoRuta(const ResultadoRuta& resultado) {
    cout << "\n=====================================\n";
    cout << "RESULTADO: " << resultado.tecnica << "\n";
    cout << "=====================================\n";

    if (!resultado.rutaValida) {
        cout << "No se encontro una ruta valida.\n";
        return;
    }

    cout << "Ruta encontrada:\n";

    for (const Entrega& entrega : resultado.ruta) {
        cout << "Entrega ID: " << entrega.id
             << " | Peso: " << entrega.peso << " kg"
             << " | Destino: ("
             << entrega.destino.fila << ", "
             << entrega.destino.columna << ")\n";
    }

    cout << "\nCosto total: " << resultado.costoTotal << endl;
    cout << "Peso total: " << calcularPesoTotalRuta(resultado.ruta) << " kg\n";

    cout << "\nInterpretacion:\n";

    if (resultado.tecnica.find("Greedy") != string::npos) {
        cout << "Greedy selecciona siempre la estacion mas cercana desde la posicion actual.\n";
        cout << "Es rapido, pero no garantiza la solucion optima absoluta.\n";
    }
    else if (resultado.tecnica == "Backtracking") {
        cout << "Backtracking prueba todas las combinaciones posibles y conserva la de menor costo.\n";
        cout << "Puede encontrar la mejor solucion, pero se vuelve costoso con muchas entregas.\n";
    }
    else if (resultado.tecnica.find("Programacion Dinamica") != string::npos) {
        cout << "Programacion Dinamica guarda resultados de subproblemas para encontrar una ruta de costo minimo.\n";
    }
    else if (resultado.tecnica == "Algoritmo Genetico") {
        cout << "El algoritmo genetico genera una poblacion de rutas y la mejora por generaciones.\n";
    }
    else if (resultado.tecnica == "Monte Carlo") {
        cout << "Monte Carlo genera muchas rutas aleatorias y conserva la mejor encontrada.\n";
    }
    else if (resultado.tecnica == "Las Vegas") {
        cout << "Las Vegas genera rutas aleatorias hasta encontrar una ruta valida.\n";
    }
}