#include <vector>
#include <algorithm>
#include <random>
#include <limits>

#include "../../Include/Genetico.h"

using namespace std;

ResultadoRuta algoritmoGenetico(
    const vector<vector<int>>& tablero,
    Punto inicio,
    vector<Entrega> entregas,
    float capacidadMaxima,
    int poblacionTam,
    int generaciones
) {
    ResultadoRuta resultado;
    resultado.costoTotal = numeric_limits<int>::max();
    resultado.rutaValida = false;
    resultado.tecnica = "Algoritmo Genetico";

    if (!validarPesoRuta(entregas, capacidadMaxima)) {
        return resultado;
    }

    random_device rd;
    mt19937 gen(rd());

    vector<vector<Entrega>> poblacion;

    for (int i = 0; i < poblacionTam; i++) {
        vector<Entrega> individuo = entregas;
        shuffle(individuo.begin(), individuo.end(), gen);
        poblacion.push_back(individuo);
    }

    for (int g = 0; g < generaciones; g++) {
        sort(
            poblacion.begin(),
            poblacion.end(),
            [&](const vector<Entrega>& a, const vector<Entrega>& b) {
                int costoA = calcularCostoRutaGeneral(tablero, inicio, a);
                int costoB = calcularCostoRutaGeneral(tablero, inicio, b);

                if (costoA == -1) {
                    costoA = numeric_limits<int>::max();
                }

                if (costoB == -1) {
                    costoB = numeric_limits<int>::max();
                }

                return costoA < costoB;
            }
        );

        int costoMejor = calcularCostoRutaGeneral(
            tablero,
            inicio,
            poblacion[0]
        );

        if (costoMejor != -1 && costoMejor < resultado.costoTotal) {
            resultado.ruta = poblacion[0];
            resultado.costoTotal = costoMejor;
            resultado.rutaValida = true;
        }

        vector<vector<Entrega>> nuevaPoblacion;

        int elite = max(1, poblacionTam / 4);

        for (int i = 0; i < elite; i++) {
            nuevaPoblacion.push_back(poblacion[i]);
        }

        while ((int)nuevaPoblacion.size() < poblacionTam) {
            vector<Entrega> hijo = poblacion[gen() % elite];

            if (hijo.size() >= 2) {
                int a = gen() % hijo.size();
                int b = gen() % hijo.size();
                swap(hijo[a], hijo[b]);
            }

            nuevaPoblacion.push_back(hijo);
        }

        poblacion = nuevaPoblacion;
    }

    return resultado;
}