#include <vector>
#include <limits>

#include "../../Include/Greedy.h"

using namespace std;

ResultadoRuta algoritmoGreedy(
    const vector<vector<int>>& tablero,
    Punto inicio,
    vector<Entrega> entregas,
    float capacidadMaxima
) {
    ResultadoRuta resultado;
    resultado.costoTotal = 0;
    resultado.rutaValida = false;
    resultado.tecnica = "Greedy - Nearest Neighbor";

    if (!validarPesoRuta(entregas, capacidadMaxima)) {
        return resultado;
    }

    vector<Entrega> pendientes = entregas;
    vector<Entrega> rutaFinal;

    Punto actual = inicio;

    while (!pendientes.empty()) {
        int mejorIndice = -1;
        int mejorDistancia = numeric_limits<int>::max();

        for (int i = 0; i < (int)pendientes.size(); i++) {
            int distancia = calcularDistanciaBFSRuta(
                tablero,
                actual,
                pendientes[i].destino
            );

            if (distancia != -1 && distancia < mejorDistancia) {
                mejorDistancia = distancia;
                mejorIndice = i;
            }
        }

        if (mejorIndice == -1) {
            resultado.rutaValida = false;
            return resultado;
        }

        rutaFinal.push_back(pendientes[mejorIndice]);
        actual = pendientes[mejorIndice].destino;
        pendientes.erase(pendientes.begin() + mejorIndice);
    }

    int costo = calcularCostoRutaGeneral(tablero, inicio, rutaFinal);

    if (costo != -1) {
        resultado.ruta = rutaFinal;
        resultado.costoTotal = costo;
        resultado.rutaValida = true;
    }

    return resultado;
}