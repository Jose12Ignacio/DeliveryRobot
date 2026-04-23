#include <iostream>
#include <vector>

using namespace std;

struct ResultadoDV {
    bool tieneInicio;
    bool tieneEstacion;
    bool conectaArriba;
    bool conectaAbajo;
    bool conectaIzquierda;
    bool conectaDerecha;
};

ResultadoDV resolverCuadrante(const vector<vector<int>>& tablero, int fi, int ff, int ci, int cf) {
    ResultadoDV r = {false, false, false, false, false, false};

    int alto = ff - fi + 1;
    int ancho = cf - ci + 1;

    if (alto <= 2 || ancho <= 2) {
        for (int i = fi; i <= ff; i++) {
            for (int j = ci; j <= cf; j++) {
                if (tablero[i][j] == 2) r.tieneInicio = true;
                if (tablero[i][j] == 3) r.tieneEstacion = true;

                if (tablero[i][j] != 0) {
                    if (i == fi) r.conectaArriba = true;
                    if (i == ff) r.conectaAbajo = true;
                    if (j == ci) r.conectaIzquierda = true;
                    if (j == cf) r.conectaDerecha = true;
                }
            }
        }
        return r;
    }

    int fm = (fi + ff) / 2;
    int cm = (ci + cf) / 2;

    ResultadoDV q1 = resolverCuadrante(tablero, fi, fm, ci, cm);
    ResultadoDV q2 = resolverCuadrante(tablero, fi, fm, cm + 1, cf);
    ResultadoDV q3 = resolverCuadrante(tablero, fm + 1, ff, ci, cm);
    ResultadoDV q4 = resolverCuadrante(tablero, fm + 1, ff, cm + 1, cf);

    r.tieneInicio = q1.tieneInicio || q2.tieneInicio || q3.tieneInicio || q4.tieneInicio;
    r.tieneEstacion = q1.tieneEstacion || q2.tieneEstacion || q3.tieneEstacion || q4.tieneEstacion;
    r.conectaArriba = q1.conectaArriba || q2.conectaArriba;
    r.conectaAbajo = q3.conectaAbajo || q4.conectaAbajo;
    r.conectaIzquierda = q1.conectaIzquierda || q3.conectaIzquierda;
    r.conectaDerecha = q2.conectaDerecha || q4.conectaDerecha;

    return r;
}

void imprimirResultadoDV(const ResultadoDV& r) {
    cout << "\nRESULTADO DIVIDE Y VENCERAS:\n";
    cout << "Tiene inicio: " << (r.tieneInicio ? "Si" : "No") << endl;
    cout << "Tiene estacion: " << (r.tieneEstacion ? "Si" : "No") << endl;
    cout << "Conexion arriba: " << (r.conectaArriba ? "Si" : "No") << endl;
    cout << "Conexion abajo: " << (r.conectaAbajo ? "Si" : "No") << endl;
    cout << "Conexion izquierda: " << (r.conectaIzquierda ? "Si" : "No") << endl;
    cout << "Conexion derecha: " << (r.conectaDerecha ? "Si" : "No") << endl;
}