#include "menu/Menu.h"
#include <iostream>

// Aquí se irán agregando los mains de cada algoritmo
// cuando tus compañeros los tengan listos

int main() {
    GLFWwindow* window = inicializarVentana();
    if (!window) {
        std::cerr << "Error: no se pudo crear la ventana.\n";
        return -1;
    }

    inicializarImGui(window);

    AppState app;
    app.tablero.assign(20, std::vector<int>(10, 0));

    loopPrincipal(window, app);
    limpiar(window);

    return 0;
}