#include <iostream>
#include <vector>
#include "Estructura.h"

using namespace std;

// 1 = Camino
// 2 = Inicio
// 3 = Estacion / Entrega
bool esTransitableDV(int valor) {
    return valor == 1 || valor == 2 || valor == 3;
}

ResultadoDV resolverCuadrante(const vector<vector<int>>& tablero,
                              int fi,
                              int ff,
                              int ci,
                              int cf) {
    ResultadoDV r = {
        false,  // tieneInicio
        false,  // tieneEstacion
        false,  // conectaArriba
        false,  // conectaAbajo
        false,  // conectaIzquierda
        false,  // conectaDerecha
        0,      // cantidadCaminos
        0       // cantidadEstaciones
    };

    int alto = ff - fi + 1;
    int ancho = cf - ci + 1;

    // Caso base: cuadrante pequeño
    if (alto <= 2 || ancho <= 2) {
        for (int i = fi; i <= ff; i++) {
            for (int j = ci; j <= cf; j++) {

                if (tablero[i][j] == 2) {
                    r.tieneInicio = true;
                }

                if (tablero[i][j] == 3) {
                    r.tieneEstacion = true;
                    r.cantidadEstaciones++;
                }

                if (esTransitableDV(tablero[i][j])) {
                    r.cantidadCaminos++;

                    if (i == fi) {
                        r.conectaArriba = true;
                    }

                    if (i == ff) {
                        r.conectaAbajo = true;
                    }

                    if (j == ci) {
                        r.conectaIzquierda = true;
                    }

                    if (j == cf) {
                        r.conectaDerecha = true;
                    }
                }
            }
        }

        return r;
    }

    // Dividir en 4 cuadrantes
    int fm = (fi + ff) / 2;
    int cm = (ci + cf) / 2;

    ResultadoDV q1 = resolverCuadrante(tablero, fi, fm, ci, cm);
    ResultadoDV q2 = resolverCuadrante(tablero, fi, fm, cm + 1, cf);
    ResultadoDV q3 = resolverCuadrante(tablero, fm + 1, ff, ci, cm);
    ResultadoDV q4 = resolverCuadrante(tablero, fm + 1, ff, cm + 1, cf);

    // Combinar resultados
    r.tieneInicio = q1.tieneInicio || q2.tieneInicio ||
                    q3.tieneInicio || q4.tieneInicio;

    r.tieneEstacion = q1.tieneEstacion || q2.tieneEstacion ||
                      q3.tieneEstacion || q4.tieneEstacion;

    r.cantidadCaminos = q1.cantidadCaminos + q2.cantidadCaminos +
                        q3.cantidadCaminos + q4.cantidadCaminos;

    r.cantidadEstaciones = q1.cantidadEstaciones + q2.cantidadEstaciones +
                           q3.cantidadEstaciones + q4.cantidadEstaciones;

    r.conectaArriba = q1.conectaArriba || q2.conectaArriba;
    r.conectaAbajo = q3.conectaAbajo || q4.conectaAbajo;
    r.conectaIzquierda = q1.conectaIzquierda || q3.conectaIzquierda;
    r.conectaDerecha = q2.conectaDerecha || q4.conectaDerecha;

    return r;
}

void imprimirResultadoDV(const ResultadoDV& r) {
    cout << "\nRESULTADO DIVIDE Y VENCERAS:\n";

    cout << "Tiene inicio: "
         << (r.tieneInicio ? "Si" : "No") << endl;

    cout << "Tiene estacion: "
         << (r.tieneEstacion ? "Si" : "No") << endl;

    cout << "Cantidad de caminos transitables: "
         << r.cantidadCaminos << endl;

    cout << "Cantidad de estaciones: "
         << r.cantidadEstaciones << endl;

    cout << "Conexion arriba: "
         << (r.conectaArriba ? "Si" : "No") << endl;

    cout << "Conexion abajo: "
         << (r.conectaAbajo ? "Si" : "No") << endl;

    cout << "Conexion izquierda: "
         << (r.conectaIzquierda ? "Si" : "No") << endl;

    cout << "Conexion derecha: "
         << (r.conectaDerecha ? "Si" : "No") << endl;

    cout << "\nInterpretacion:\n";

    if (r.tieneInicio && r.tieneEstacion && r.cantidadCaminos > 0) {
        cout << "El mapa contiene inicio, estaciones y caminos transitables.\n";
        cout << "Divide y Venceras dividio el mapa en cuadrantes, resolvio cada subregion y combino los resultados.\n";
    }
    else {
        cout << "El mapa no contiene todos los elementos necesarios para una ruta completa.\n";
    }
}