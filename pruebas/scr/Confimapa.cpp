#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>

#include "Estructura.h"

using namespace std;

bool esTransitable(int valor) {
    return valor == 1 || valor == 2 || valor == 3;
}

int extraerNumero(const string& linea, const string& clave) {
    size_t pos = linea.find(clave);

    if (pos == string::npos) {
        return -1;
    }

    pos = linea.find(":", pos);

    if (pos == string::npos) {
        return -1;
    }

    pos++;

    while (pos < linea.size() && linea[pos] == ' ') {
        pos++;
    }

    string numero = "";

    while (pos < linea.size() && isdigit(linea[pos])) {
        numero += linea[pos];
        pos++;
    }

    if (numero.empty()) {
        return -1;
    }

    return stoi(numero);
}

string extraerTexto(const string& linea, const string& clave) {
    size_t pos = linea.find(clave);

    if (pos == string::npos) {
        return "";
    }

    pos = linea.find(":", pos);

    if (pos == string::npos) {
        return "";
    }

    pos = linea.find("\"", pos);

    if (pos == string::npos) {
        return "";
    }

    pos++;

    size_t fin = linea.find("\"", pos);

    if (fin == string::npos) {
        return "";
    }

    return linea.substr(pos, fin - pos);
}

void cargarMapaDesdeJSON(const string& ruta,
                         vector<vector<int>>& tablero,
                         Punto& inicio,
                         vector<Punto>& estaciones) {
    ifstream archivo(ruta);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo JSON en la ruta: "
             << ruta << endl;
        return;
    }

    string linea;

    while (getline(archivo, linea)) {
        if (linea.find("\"fila\"") != string::npos) {
            int fila = extraerNumero(linea, "\"fila\"");
            int columna = extraerNumero(linea, "\"columna\"");
            string valor = extraerTexto(linea, "\"valor\"");

            if (fila >= 1 && fila <= (int)tablero.size() &&
                columna >= 1 && columna <= (int)tablero[0].size()) {

                if (valor == "Camino") {
                    tablero[fila - 1][columna - 1] = 1;
                }
                else if (valor == "Inicio") {
                    tablero[fila - 1][columna - 1] = 2;
                    inicio = {fila - 1, columna - 1};
                }
                else if (valor == "Estacion") {
                    tablero[fila - 1][columna - 1] = 3;
                    estaciones.push_back({fila - 1, columna - 1});
                }
            }
        }
    }

    archivo.close();
}

vector<Punto> obtenerVecinos(const vector<vector<int>>& tablero, Punto p) {
    vector<Punto> vecinos;

    if (p.fila < 0 || p.columna < 0) {
        return vecinos;
    }

    int filas = tablero.size();
    int columnas = tablero[0].size();

    int df[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    for (int k = 0; k < 4; k++) {
        int nuevaFila = p.fila + df[k];
        int nuevaColumna = p.columna + dc[k];

        if (nuevaFila >= 0 && nuevaFila < filas &&
            nuevaColumna >= 0 && nuevaColumna < columnas &&
            esTransitable(tablero[nuevaFila][nuevaColumna])) {

            vecinos.push_back({nuevaFila, nuevaColumna});
        }
    }

    return vecinos;
}

vector<NodoGrafo> generarGrafo(const vector<vector<int>>& tablero) {
    vector<NodoGrafo> grafo;

    for (int i = 0; i < (int)tablero.size(); i++) {
        for (int j = 0; j < (int)tablero[i].size(); j++) {

            if (esTransitable(tablero[i][j])) {
                Punto actual = {i, j};

                NodoGrafo nodo;
                nodo.posicion = actual;
                nodo.vecinos = obtenerVecinos(tablero, actual);

                grafo.push_back(nodo);
            }
        }
    }

    return grafo;
}

void imprimirMapa(const vector<vector<int>>& tablero) {
    cout << "\nMAPA CARGADO:\n\n";

    for (int i = 0; i < (int)tablero.size(); i++) {
        for (int j = 0; j < (int)tablero[i].size(); j++) {
            cout << tablero[i][j] << " ";
        }

        cout << endl;
    }
}

void imprimirVecinos(const vector<Punto>& vecinos) {
    if (vecinos.empty()) {
        cout << "No hay vecinos transitables.\n";
        return;
    }

    for (size_t i = 0; i < vecinos.size(); i++) {
        cout << "(" << vecinos[i].fila << ", "
             << vecinos[i].columna << ")\n";
    }
}

void imprimirGrafo(const vector<NodoGrafo>& grafo) {
    cout << "\nGRAFO GENERADO:\n\n";

    for (const NodoGrafo& nodo : grafo) {
        cout << "Nodo (" << nodo.posicion.fila << ", "
             << nodo.posicion.columna << ") -> ";

        if (nodo.vecinos.empty()) {
            cout << "Sin vecinos";
        }
        else {
            for (const Punto& vecino : nodo.vecinos) {
                cout << "(" << vecino.fila << ", "
                     << vecino.columna << ") ";
            }
        }

        cout << endl;
    }
}

void exportarGrafoDOT(const vector<NodoGrafo>& grafo,
                      const vector<vector<int>>& tablero,
                      const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "No se pudo crear el archivo DOT.\n";
        return;
    }

    archivo << "graph Mapa {\n";
    archivo << "    layout=neato;\n";
    archivo << "    overlap=false;\n";
    archivo << "    splines=true;\n";
    archivo << "    node [shape=circle, style=filled, fillcolor=lightgray];\n";

    int contadorEntregas = 1;

    for (const NodoGrafo& nodo : grafo) {
        int fila = nodo.posicion.fila;
        int columna = nodo.posicion.columna;

        string nodoID = to_string(fila) + "_" + to_string(columna);

        if (tablero[fila][columna] == 2) {
            archivo << "    \"" << nodoID << "\" "
                    << "[label=\"Inicio\", shape=doublecircle, fillcolor=lightgreen];\n";
        }
        else if (tablero[fila][columna] == 3) {
            archivo << "    \"" << nodoID << "\" "
                    << "[label=\"Entrega " << contadorEntregas
                    << "\", shape=box, fillcolor=lightblue];\n";

            contadorEntregas++;
        }
        else {
            archivo << "    \"" << nodoID << "\" "
                    << "[label=\"(" << fila << "," << columna << ")\"];\n";
        }
    }

    for (const NodoGrafo& nodo : grafo) {
        string nodoActual = to_string(nodo.posicion.fila) + "_" +
                            to_string(nodo.posicion.columna);

        for (const Punto& vecino : nodo.vecinos) {
            string nodoVecino = to_string(vecino.fila) + "_" +
                                to_string(vecino.columna);

            if (nodoActual < nodoVecino) {
                archivo << "    \"" << nodoActual << "\" -- \""
                        << nodoVecino << "\";\n";
            }
        }
    }

    archivo << "}\n";

    archivo.close();

    cout << "\nArchivo DOT generado correctamente: "
         << nombreArchivo << endl;
}