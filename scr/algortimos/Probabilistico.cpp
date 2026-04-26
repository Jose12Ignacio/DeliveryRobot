#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <limits>
#include <ctime>

#include "../../Include/Estructura.h"

using namespace std;

// 1 = Camino
// 2 = Inicio
// 3 = Estacion
bool esTransitableProb(int valor) {
    return valor == 1 || valor == 2 || valor == 3;
}

struct ResultadoProbabilistico {
    vector<Entrega> ruta;
    int costoTotal;
    bool rutaValida;
    string tecnica;
};

// Calcula distancia mínima entre dos puntos usando BFS
int calcularDistanciaBFS(const vector<vector<int>>& tablero, Punto origen, Punto destino) {
    if (tablero.empty()) {
        return -1;
    }

    int filas = tablero.size();
    int columnas = tablero[0].size();

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

        if (actual.fila == destino.fila && actual.columna == destino.columna) {
            return distancia[actual.fila][actual.columna];
        }

        for (int i = 0; i < 4; i++) {
            int nf = actual.fila + df[i];
            int nc = actual.columna + dc[i];

            if (nf >= 0 && nf < filas &&
                nc >= 0 && nc < columnas &&
                !visitado[nf][nc] &&
                esTransitableProb(tablero[nf][nc])) {

                visitado[nf][nc] = true;
                distancia[nf][nc] = distancia[actual.fila][actual.columna] + 1;
                cola.push({nf, nc});
            }
        }
    }

    return -1;
}

// Calcula el costo total de una ruta:
// Inicio -> entrega 1 -> entrega 2 -> ... -> Inicio
int calcularCostoRuta(const vector<vector<int>>& tablero,
                      Punto inicio,
                      const vector<Entrega>& ruta) {
    if (ruta.empty()) {
        return 0;
    }

    int costoTotal = 0;
    Punto actual = inicio;

    for (const Entrega& entrega : ruta) {
        int distancia = calcularDistanciaBFS(tablero, actual, entrega.destino);

        if (distancia == -1) {
            return -1;
        }

        costoTotal += distancia;
        actual = entrega.destino;
    }

    int regreso = calcularDistanciaBFS(tablero, actual, inicio);

    if (regreso == -1) {
        return -1;
    }

    costoTotal += regreso;

    return costoTotal;
}

// Valida que el peso total no supere 20 kg
bool validarPeso(const vector<Entrega>& ruta, float capacidadMaxima) {
    float pesoTotal = 0;

    for (const Entrega& entrega : ruta) {
        pesoTotal += entrega.peso;
    }

    return pesoTotal <= capacidadMaxima;
}

// Algoritmo Monte Carlo
ResultadoProbabilistico algoritmoMonteCarlo(const vector<vector<int>>& tablero,
                                            Punto inicio,
                                            vector<Entrega> entregas,
                                            int iteraciones,
                                            float capacidadMaxima) {
    ResultadoProbabilistico mejorResultado;
    mejorResultado.costoTotal = numeric_limits<int>::max();
    mejorResultado.rutaValida = false;
    mejorResultado.tecnica = "Monte Carlo";

    random_device rd;
    mt19937 generador(rd());

    for (int i = 0; i < iteraciones; i++) {
        shuffle(entregas.begin(), entregas.end(), generador);

        if (!validarPeso(entregas, capacidadMaxima)) {
            continue;
        }

        int costo = calcularCostoRuta(tablero, inicio, entregas);

        if (costo != -1 && costo < mejorResultado.costoTotal) {
            mejorResultado.ruta = entregas;
            mejorResultado.costoTotal = costo;
            mejorResultado.rutaValida = true;
        }
    }

    return mejorResultado;
}

// Algoritmo Las Vegas
ResultadoProbabilistico algoritmoLasVegas(const vector<vector<int>>& tablero,
                                          Punto inicio,
                                          vector<Entrega> entregas,
                                          int maxIntentos,
                                          float capacidadMaxima) {
    ResultadoProbabilistico resultado;
    resultado.costoTotal = numeric_limits<int>::max();
    resultado.rutaValida = false;
    resultado.tecnica = "Las Vegas";

    random_device rd;
    mt19937 generador(rd());

    int intentos = 0;

    while (intentos < maxIntentos) {
        shuffle(entregas.begin(), entregas.end(), generador);

        if (validarPeso(entregas, capacidadMaxima)) {
            int costo = calcularCostoRuta(tablero, inicio, entregas);

            if (costo != -1) {
                resultado.ruta = entregas;
                resultado.costoTotal = costo;
                resultado.rutaValida = true;
                return resultado;
            }
        }

        intentos++;
    }

    return resultado;
}

void imprimirResultadoProbabilistico(const ResultadoProbabilistico& resultado) {
    cout << "\nRESULTADO ALGORITMO PROBABILISTICO: "
         << resultado.tecnica << "\n";

    if (!resultado.rutaValida) {
        cout << "No se encontro una ruta valida.\n";
        return;
    }

    cout << "Ruta encontrada:\n";

    for (const Entrega& entrega : resultado.ruta) {
        cout << "Entrega ID: " << entrega.id
             << " | Peso: " << entrega.peso
             << " kg | Destino: ("
             << entrega.destino.fila << ", "
             << entrega.destino.columna << ")\n";
    }

    cout << "Costo total de la ruta: "
         << resultado.costoTotal << endl;

    cout << "\nInterpretacion:\n";

    if (resultado.tecnica == "Monte Carlo") {
        cout << "Monte Carlo genero muchas rutas aleatorias y selecciono la mejor encontrada.\n";
        cout << "No garantiza encontrar la solucion optima absoluta, pero puede encontrar una buena solucion usando muchas iteraciones.\n";
    }
    else if (resultado.tecnica == "Las Vegas") {
        cout << "Las Vegas genero rutas aleatorias hasta encontrar una ruta valida.\n";
        cout << "La solucion encontrada siempre es valida, pero el tiempo puede variar dependiendo de los intentos necesarios.\n";
    }
}