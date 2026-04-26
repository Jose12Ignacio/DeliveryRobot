#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

// ─────────────────────────────────────────────
//  Structs base (temporal hasta tener Structs.h)
// ─────────────────────────────────────────────
struct Punto {
    int fila;
    int columna;
};

struct Entrega {
    int         id;
    float       peso;
    Punto       destino;
};

// ─────────────────────────────────────────────
//  Estado global de la aplicación
// ─────────────────────────────────────────────
struct AppState {
    // Mapa
    std::vector<std::vector<int>> tablero;
    Punto                         inicio        = {-1, -1};
    std::vector<Punto>            estaciones;
    bool                          mapaListo     = false;

    // Entregas
    std::vector<Entrega>          entregas;
    int                           nextId        = 1;

    // Configuración
    int                           maxDeliveries = 1;
    bool                          algoDP        = false;
    bool                          algoGenetico  = false;
    bool                          algoGreedy    = false;
    bool                          algoBacktrack = false;
    bool                          algoDyV       = false;
    bool                          algoMonteCarlo= false;

    // Pantalla activa
    // 0=Principal 1=Registro 2=Config 3=Preview 4=Enviar
    int pantalla = 0;
};

// ─────────────────────────────────────────────
//  Funciones del menú (pantallas)
// ─────────────────────────────────────────────
void mostrarMenuPrincipal(AppState& app);
void mostrarRegistro     (AppState& app);
void mostrarConfiguracion(AppState& app);
void mostrarPreview      (AppState& app);
void mostrarEnviar       (AppState& app);

// ─────────────────────────────────────────────
//  Ciclo de vida de la ventana
// ─────────────────────────────────────────────
GLFWwindow* inicializarVentana();
void        inicializarImGui  (GLFWwindow* window);
void        loopPrincipal     (GLFWwindow* window, AppState& app);
void        limpiar           (GLFWwindow* window);