#pragma once

#include <vector>

struct Punto {
    int fila;
    int columna;
};

struct NodoGrafo {
    Punto posicion;
    std::vector<Punto> vecinos;
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

struct Entrega {
    int id;
    float peso;
    Punto destino;
};