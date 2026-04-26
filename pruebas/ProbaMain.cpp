#include <iostream>
#include <vector>
#include <string>

#include "../Include/Estructura.h"

using namespace std;

// Funciones de Confimapa.cpp
void cargarMapaDesdeJSON(const string& ruta,
                         vector<vector<int>>& tablero,
                         Punto& inicio,
                         vector<Punto>& estaciones);

void imprimirMapa(const vector<vector<int>>& tablero);

// Struct del resultado probabilistico
struct ResultadoProbabilistico {
    vector<Entrega> ruta;
    int costoTotal;
    bool rutaValida;
    string tecnica;
};

// Funciones de Probabilisticos.cpp
ResultadoProbabilistico algoritmoMonteCarlo(const vector<vector<int>>& tablero,
                                            Punto inicio,
                                            vector<Entrega> entregas,
                                            int iteraciones,
                                            float capacidadMaxima);

ResultadoProbabilistico algoritmoLasVegas(const vector<vector<int>>& tablero,
                                          Punto inicio,
                                          vector<Entrega> entregas,
                                          int maxIntentos,
                                          float capacidadMaxima);

void imprimirResultadoProbabilistico(const ResultadoProbabilistico& resultado);

int main() {

    cout << "=====================================\n";
    cout << " PRUEBA DE ALGORITMOS PROBABILISTICOS\n";
    cout << " Monte Carlo y Las Vegas\n";
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
        cout << "Revisa que el archivo JSON exista en: " << rutaJson << endl;
        return 1;
    }

    if (estaciones.empty()) {
        cout << "Error: no se encontraron estaciones de entrega.\n";
        return 1;
    }

    cout << "Mapa cargado correctamente.\n";
    cout << "Inicio: (" << inicio.fila << ", " << inicio.columna << ")\n";
    cout << "Cantidad de estaciones: " << estaciones.size() << "\n\n";

    imprimirMapa(tablero);

    vector<Entrega> entregas;

    int cantidadEntregas;

    cout << "\nCuantas entregas desea registrar?: ";
    cin >> cantidadEntregas;

    for (int i = 0; i < cantidadEntregas; i++) {
        Entrega nueva;

        nueva.id = i + 1;

        cout << "\nEntrega #" << nueva.id << endl;

        cout << "Peso del paquete: ";
        cin >> nueva.peso;

        cout << "Fila destino: ";
        cin >> nueva.destino.fila;

        cout << "Columna destino: ";
        cin >> nueva.destino.columna;

        bool esEstacion = false;

        for (const Punto& e : estaciones) {
            if (e.fila == nueva.destino.fila &&
                e.columna == nueva.destino.columna) {
                esEstacion = true;
                break;
            }
        }

        if (!esEstacion) {
            cout << "Error: la posicion ("
                 << nueva.destino.fila << ", "
                 << nueva.destino.columna
                 << ") no es una estacion valida.\n";

            cout << "Esta entrega no se agrego.\n";
        }
        else if (nueva.peso <= 0 || nueva.peso > 20) {
            cout << "Error: el peso debe estar entre 0 y 20 kg.\n";
            cout << "Esta entrega no se agrego.\n";
        }
        else {
            entregas.push_back(nueva);
            cout << "Entrega agregada correctamente.\n";
        }
    }

    if (entregas.empty()) {
        cout << "\nNo hay entregas validas para procesar.\n";
        return 1;
    }

    float capacidadMaxima = 20.0f;

    int iteracionesMonteCarlo = 1000;
    int intentosLasVegas = 1000;

    cout << "\n=====================================\n";
    cout << " EJECUTANDO MONTE CARLO\n";
    cout << "=====================================\n";

    ResultadoProbabilistico resultadoMC = algoritmoMonteCarlo(
        tablero,
        inicio,
        entregas,
        iteracionesMonteCarlo,
        capacidadMaxima
    );

    imprimirResultadoProbabilistico(resultadoMC);

    cout << "\n=====================================\n";
    cout << " EJECUTANDO LAS VEGAS\n";
    cout << "=====================================\n";

    ResultadoProbabilistico resultadoLV = algoritmoLasVegas(
        tablero,
        inicio,
        entregas,
        intentosLasVegas,
        capacidadMaxima
    );

    imprimirResultadoProbabilistico(resultadoLV);

    cout << "\nFin de la prueba probabilistica.\n";

    return 0;
}