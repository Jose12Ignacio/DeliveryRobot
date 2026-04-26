#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>

#include "../../Include/ProgramacionDinamica.h"

using namespace std;

ResultadoRuta algoritmoProgramacionDinamica(
    const vector<vector<int>>& tablero,
    Punto inicio,
    vector<Entrega> entregas,
    float capacidadMaxima
) {
    ResultadoRuta resultado;
    resultado.costoTotal = numeric_limits<int>::max();
    resultado.rutaValida = false;
    resultado.tecnica = "Programacion Dinamica - Ruta Optima";

    int n = entregas.size();

    if (n == 0) {
        resultado.costoTotal = 0;
        resultado.rutaValida = true;
        return resultado;
    }

    if (!validarPesoRuta(entregas, capacidadMaxima)) {
        return resultado;
    }

    vector<vector<int>> dist(n + 1, vector<int>(n + 1, -1));

    for (int i = 0; i < n; i++) {
        dist[0][i + 1] = calcularDistanciaBFSRuta(
            tablero,
            inicio,
            entregas[i].destino
        );

        dist[i + 1][0] = calcularDistanciaBFSRuta(
            tablero,
            entregas[i].destino,
            inicio
        );
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                dist[i + 1][j + 1] = calcularDistanciaBFSRuta(
                    tablero,
                    entregas[i].destino,
                    entregas[j].destino
                );
            }
        }
    }

    int totalEstados = 1 << n;

    vector<vector<int>> dp(
        totalEstados,
        vector<int>(n, numeric_limits<int>::max())
    );

    vector<vector<int>> padre(totalEstados, vector<int>(n, -1));

    for (int i = 0; i < n; i++) {
        if (dist[0][i + 1] != -1) {
            dp[1 << i][i] = dist[0][i + 1];
        }
    }

    for (int mask = 0; mask < totalEstados; mask++) {
        for (int ultimo = 0; ultimo < n; ultimo++) {
            if (dp[mask][ultimo] == numeric_limits<int>::max()) {
                continue;
            }

            for (int siguiente = 0; siguiente < n; siguiente++) {
                if (mask & (1 << siguiente)) {
                    continue;
                }

                if (dist[ultimo + 1][siguiente + 1] == -1) {
                    continue;
                }

                int nuevoMask = mask | (1 << siguiente);
                int nuevoCosto =
                    dp[mask][ultimo] + dist[ultimo + 1][siguiente + 1];

                if (nuevoCosto < dp[nuevoMask][siguiente]) {
                    dp[nuevoMask][siguiente] = nuevoCosto;
                    padre[nuevoMask][siguiente] = ultimo;
                }
            }
        }
    }

    int maskFinal = totalEstados - 1;
    int mejorUltimo = -1;
    int mejorCosto = numeric_limits<int>::max();

    for (int ultimo = 0; ultimo < n; ultimo++) {
        if (dp[maskFinal][ultimo] == numeric_limits<int>::max()) {
            continue;
        }

        if (dist[ultimo + 1][0] == -1) {
            continue;
        }

        int costoTotal = dp[maskFinal][ultimo] + dist[ultimo + 1][0];

        if (costoTotal < mejorCosto) {
            mejorCosto = costoTotal;
            mejorUltimo = ultimo;
        }
    }

    if (mejorUltimo == -1) {
        return resultado;
    }

    vector<int> ordenIndices;

    int mask = maskFinal;
    int actual = mejorUltimo;

    while (actual != -1) {
        ordenIndices.push_back(actual);

        int anterior = padre[mask][actual];
        mask = mask ^ (1 << actual);
        actual = anterior;
    }

    reverse(ordenIndices.begin(), ordenIndices.end());

    for (int indice : ordenIndices) {
        resultado.ruta.push_back(entregas[indice]);
    }

    resultado.costoTotal = mejorCosto;
    resultado.rutaValida = true;

    return resultado;
}

ResultadoMochila mochilaProgramacionDinamica(
    vector<Entrega> entregas,
    float capacidadMaxima
) {
    ResultadoMochila resultado;
    resultado.pesoTotal = 0;
    resultado.valorTotal = 0;
    resultado.solucionValida = false;

    int n = entregas.size();

    int capacidad = (int)round(capacidadMaxima * 10);

    vector<int> pesos(n);
    vector<int> valores(n);

    for (int i = 0; i < n; i++) {
        pesos[i] = (int)round(entregas[i].peso * 10);

        // Valor simple: cada entrega vale 1.
        valores[i] = 1;
    }

    vector<vector<int>> dp(n + 1, vector<int>(capacidad + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacidad; w++) {
            dp[i][w] = dp[i - 1][w];

            if (pesos[i - 1] <= w) {
                dp[i][w] = max(
                    dp[i][w],
                    dp[i - 1][w - pesos[i - 1]] + valores[i - 1]
                );
            }
        }
    }

    int w = capacidad;

    for (int i = n; i >= 1; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            resultado.paquetesSeleccionados.push_back(entregas[i - 1]);
            resultado.pesoTotal += entregas[i - 1].peso;
            resultado.valorTotal += valores[i - 1];
            w -= pesos[i - 1];
        }
    }

    resultado.solucionValida = !resultado.paquetesSeleccionados.empty();

    return resultado;
}

void imprimirResultadoMochila(const ResultadoMochila& resultado) {
    cout << "\n=====================================\n";
    cout << "RESULTADO MOCHILA - PROGRAMACION DINAMICA\n";
    cout << "=====================================\n";

    if (!resultado.solucionValida) {
        cout << "No se seleccionaron paquetes.\n";
        return;
    }

    cout << "Paquetes seleccionados:\n";

    for (const Entrega& entrega : resultado.paquetesSeleccionados) {
        cout << "Entrega ID: " << entrega.id
             << " | Peso: " << entrega.peso << " kg"
             << " | Destino: ("
             << entrega.destino.fila << ", "
             << entrega.destino.columna << ")\n";
    }

    cout << "\nPeso total seleccionado: "
         << resultado.pesoTotal << " kg\n";

    cout << "Valor total: "
         << resultado.valorTotal << endl;

    cout << "\nInterpretacion:\n";
    cout << "La mochila selecciona paquetes sin superar la capacidad maxima del robot.\n";
    cout << "En esta version, cada paquete tiene valor 1, por lo que intenta llevar la mayor cantidad posible.\n";
}