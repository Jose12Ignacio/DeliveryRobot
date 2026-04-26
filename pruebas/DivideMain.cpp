#include <iostream>
#include <vector>
#include <string>

#include "Include/Estructura.h"

using namespace std;

// Funciones de Confimapa.cpp
void cargarMapaDesdeJSON(const string& ruta,
                         vector<vector<int>>& tablero,
                         Punto& inicio,
                         vector<Punto>& estaciones);

vector<Punto> obtenerVecinos(const vector<vector<int>>& tablero, Punto p);

vector<NodoGrafo> generarGrafo(const vector<vector<int>>& tablero);

void imprimirMapa(const vector<vector<int>>& tablero);

void imprimirVecinos(const vector<Punto>& vecinos);

void imprimirGrafo(const vector<NodoGrafo>& grafo);

void exportarGrafoDOT(const vector<NodoGrafo>& grafo,
                      const vector<vector<int>>& tablero,
                      const string& nombreArchivo);

// Funciones de Divide y venceras.cpp
ResultadoDV resolverCuadrante(const vector<vector<int>>& tablero,
                              int fi,
                              int ff,
                              int ci,
                              int cf);

void imprimirResultadoDV(const ResultadoDV& r);

int main() {

    string rutaJson = "tablero.json";

    int filas = 20;
    int columnas = 10;

    vector<vector<int>> tablero(filas, vector<int>(columnas, 0));

    Punto inicio = {-1, -1};
    vector<Punto> estaciones;

    cargarMapaDesdeJSON(rutaJson, tablero, inicio, estaciones);

    if (inicio.fila == -1 || inicio.columna == -1) {
        cout << "Error: no se encontro el punto de inicio en el mapa.\n";
        return 1;
    }

    imprimirMapa(tablero);

    cout << "\nPosicion inicial: ("
         << inicio.fila << ", "
         << inicio.columna << ")\n";

    cout << "Cantidad de puntos de entrega: "
         << estaciones.size() << "\n";

    vector<Punto> vecinosInicio = obtenerVecinos(tablero, inicio);

    cout << "\nVecinos del punto inicial:\n";
    imprimirVecinos(vecinosInicio);

    vector<NodoGrafo> grafo = generarGrafo(tablero);

    imprimirGrafo(grafo);

    exportarGrafoDOT(grafo, tablero, "grafo.dot");

    ResultadoDV resultado = resolverCuadrante(
        tablero,
        0,
        filas - 1,
        0,
        columnas - 1
    );

    imprimirResultadoDV(resultado);

    return 0;
}