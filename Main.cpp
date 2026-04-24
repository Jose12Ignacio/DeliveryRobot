#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Punto {
    int fila;
    int columna;
};

struct NodoGrafo {
    Punto posicion;
    vector<Punto> vecinos;
};

struct ResultadoDV {
    bool tieneInicio;
    bool tieneEstacion;
    bool conectaArriba;
    bool conectaAbajo;
    bool conectaIzquierda;
    bool conectaDerecha;
    int cantidadCaminos;
    int cantidadEstaciones;
};

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
    const int filas = 20;
    const int columnas = 10;

    vector<vector<int>> tablero(filas, vector<int>(columnas, 0));

    Punto inicio = {-1, -1};
    vector<Punto> estaciones;

    string rutaJson = "C:\\Users\\josei\\OneDrive\\Escritorio\\TEC\\Semestre 3\\Proyecto 2\\tablero.json";

    cargarMapaDesdeJSON(rutaJson, tablero, inicio, estaciones);

    imprimirMapa(tablero);

    cout << "\nPosicion inicial: (" << inicio.fila << ", "
         << inicio.columna << ")\n";

    cout << "Cantidad de puntos de entrega: " << estaciones.size() << "\n";

    vector<Punto> vecinosInicio = obtenerVecinos(tablero, inicio);

    cout << "\nVecinos del punto inicial:\n";
    imprimirVecinos(vecinosInicio);

    vector<NodoGrafo> grafo = generarGrafo(tablero);

    imprimirGrafo(grafo);

    exportarGrafoDOT(grafo, tablero, "grafo.dot");

    ResultadoDV resultado = resolverCuadrante(tablero, 0, filas - 1, 0, columnas - 1);

    imprimirResultadoDV(resultado);

    return 0;
}