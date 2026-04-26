#include <iostream>
#include <vector>
#include <string>

#include "../Include/Estructura.h"
#include "../Include/Dividevenceras.h"

using namespace std;

// Funciones de Confimapa.cpp
void cargarMapaDesdeJSON(const string& ruta,
                         vector<vector<int>>& tablero,
                         Punto& inicio,
                         vector<Punto>& estaciones);

void imprimirMapa(const vector<vector<int>>& tablero);

int main() {
    cout << "=====================================\n";
    cout << " PRUEBA DIVIDE Y VENCERAS\n";
    cout << "=====================================\n\n";

    string rutaJson = "data/tablero.json";

    int filas = 20;
    int columnas = 10;

    vector<vector<int>> tablero(filas, vector<int>(columnas, 0));

    Punto inicio = {-1, -1};
    vector<Punto> estaciones;

    cargarMapaDesdeJSON(rutaJson, tablero, inicio, estaciones);

    if (inicio.fila == -1 || inicio.columna == -1) {
        cout << "Error: no se encontro el punto de inicio.\n";
        cout << "Revise que exista el archivo: " << rutaJson << endl;
        return 1;
    }

    if (estaciones.empty()) {
        cout << "Error: no se encontraron estaciones en el mapa.\n";
        return 1;
    }

    cout << "Mapa cargado correctamente.\n";
    cout << "Inicio: (" << inicio.fila << ", "
         << inicio.columna << ")\n";

    cout << "Cantidad de estaciones: "
         << estaciones.size() << "\n";

    imprimirMapa(tablero);

    ResultadoDV resultado = resolverCuadrante(
        tablero,
        0,
        filas - 1,
        0,
        columnas - 1
    );

    imprimirResultadoDV(resultado);

    cout << "\nFin de la prueba Divide y Venceras.\n";

    return 0;
}