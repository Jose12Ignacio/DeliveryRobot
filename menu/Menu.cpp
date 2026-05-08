//Menu.cpp
#include <algorithm>
#include <windows.h>
#include "../Include/Menu.h"
#include "../Include/RutasUtil.h"
#include <iostream>
#include <cstdio>

//Forward declaration de ConfMapa.cpp
void cargarMapaDesdeJSON(const std::string& ruta,
                         std::vector<std::vector<int>>& tablero,
                         Punto& inicio,
                         std::vector<Punto>& estaciones);

//Incialización de la ventana
GLFWwindow* inicializarVentana() {
    if (!glfwInit()) return nullptr;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080,
                                          "Smart Delivery Robot",
                                          nullptr, nullptr);
    if (!window) { glfwTerminate(); return nullptr; }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return window;
}

void inicializarImGui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImGuiStyle& style      = ImGui::GetStyle();
    style.WindowRounding   = 6.f;
    style.FrameRounding    = 4.f;
    style.ItemSpacing      = {10, 8};
    style.WindowBorderSize = 0.f;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

//LOOP PRINCIPAL
void loopPrincipal(GLFWwindow* window, AppState& app) {
    ImGuiIO& io = ImGui::GetIO();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //Fondo completo
        ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
        ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.07f, 0.07f, 0.11f, 1.f});
        ImGui::Begin("##fondo", nullptr,
                    ImGuiWindowFlags_NoDecoration |
                    ImGuiWindowFlags_NoInputs     |
                    ImGuiWindowFlags_NoBackground |
                    ImGuiWindowFlags_NoBringToFrontOnFocus);
        ImGui::PopStyleColor();
        ImGui::End();

        //Panel lateral siempre visible
        mostrarMenuPrincipal(app);

        //Pantalla activa
        switch (app.pantalla) {
            case 1: mostrarRegistro(app);      break;
            case 2: mostrarConfiguracion(app); break;
            case 3: mostrarPreview(app);        break;
            case 4: mostrarEnviar(app);         break;
        }

        //Render
        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.07f, 0.07f, 0.11f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

//Limpiar la ventana
void limpiar(GLFWwindow* window) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

//Pantalla de menu principal
void mostrarMenuPrincipal(AppState& app) {
    //Configuración de panel pricipal
    ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({280, 1080}, ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.11f, 0.11f, 0.18f, 1.f});
    ImGui::Begin("##panel", nullptr,
                 ImGuiWindowFlags_NoResize   |
                 ImGuiWindowFlags_NoMove     |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::PopStyleColor();

    ImGui::Spacing();
    ImGui::TextColored({0.5f, 0.8f, 1.f, 1.f}, "  Smart Delivery");
    ImGui::TextColored({0.6f, 0.6f, 0.8f, 1.f}, "  Robot System");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    const ImVec2 tamBoton = {240, 70};
    ImGui::PushStyleColor(ImGuiCol_Button,        {0.20f, 0.40f, 0.70f, 1.f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.30f, 0.60f, 1.00f, 1.f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  {0.10f, 0.25f, 0.50f, 1.f});

    if (ImGui::Button("1.  Registrar Paquetes", tamBoton)) app.pantalla = 1;
    ImGui::Spacing();
    if (ImGui::Button("2.  Configuracion",       tamBoton)) app.pantalla = 2;
    ImGui::Spacing();
    if (ImGui::Button("3.  Preview de Ruta",     tamBoton)) app.pantalla = 3;
    ImGui::Spacing();
    if (ImGui::Button("4.  Enviar al Robot",     tamBoton)) app.pantalla = 4;

    ImGui::PopStyleColor(3);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Estado rápido
    ImGui::TextColored({0.6f, 0.6f, 0.8f, 1.f}, "Estado:");
    ImGui::Text("Mapa: %s", app.mapaListo ? "Cargado" : "No cargado");
    ImGui::Text("Paquetes: %zu", app.entregas.size());
    ImGui::Text("Max viaje: %d", app.maxDeliveries);

    ImGui::SetCursorPosY(1040);
    ImGui::TextDisabled("v1.0  |  CE-2103");

    ImGui::End();

    //Pantalla pricnipal
    if (app.pantalla == 0) {
        ImGui::SetNextWindowPos({300, 50}, ImGuiCond_Always);
        ImGui::SetNextWindowSize({1580, 980}, ImGuiCond_Always);
        ImGui::Begin("##contenido", nullptr,
                     ImGuiWindowFlags_NoResize   |
                     ImGuiWindowFlags_NoMove     |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar);

        ImGui::SetCursorPos({600, 400});
        ImGui::TextColored({0.9f, 0.9f, 1.f, 1.f},
                           "Bienvenido al sistema de entregas");
        ImGui::SetCursorPos({480, 450});
        ImGui::TextDisabled(
            "Selecciona una opcion del panel izquierdo para comenzar");

        ImGui::End();
    }
}

//Pantalla de registro de paquetes
void mostrarRegistro(AppState& app) {
    ImGui::SetNextWindowPos({300, 50}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({1580, 980}, ImGuiCond_Always);
    ImGui::Begin("Registro de Paquetes", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);

    ImGui::TextColored({0.5f, 0.8f, 1.f, 1.f}, "Registrar nuevo paquete");
    ImGui::Separator();
    ImGui::Spacing();

    static int peso    = 0;
    static int   filaDst = 0;
    static int   colDst  = 0;
    static char  errorMsg[128] = "";

    ImGui::Text("ID asignado automaticamente: %d", app.nextId);
    ImGui::Spacing();

    ImGui::Text("Peso del paquete (max 20 kg):");
    ImGui::SetNextItemWidth(200);
    ImGui::InputInt("##peso", &peso);
    if (peso < 1) peso = 1;
    if (peso > 20) peso = 20;
    ImGui::Spacing();
   ImGui::Text("Estacion destino:");
    ImGui::Spacing();

    if (!app.mapaListo || app.estaciones.empty()) {
        ImGui::TextColored({1.f, 0.6f, 0.2f, 1.f},
                            "Carga el mapa primero en Configuracion.");
    } else {
    static int estacionSeleccionada = 0;

    // Asegurar que el índice sea válido
    if (estacionSeleccionada >= (int)app.estaciones.size())
        estacionSeleccionada = 0;

    // Construir label del combo
    std::string labelActual = "Estacion (" +
        std::to_string(app.estaciones[estacionSeleccionada].fila) + ", " +
        std::to_string(app.estaciones[estacionSeleccionada].columna) + ")";

    ImGui::SetNextItemWidth(300);
    if (ImGui::BeginCombo("##estacion", labelActual.c_str())) {
        for (int i = 0; i < (int)app.estaciones.size(); i++) {
            std::string opcion = "Estacion (" +
                std::to_string(app.estaciones[i].fila) + ", " +
                std::to_string(app.estaciones[i].columna) + ")";
            bool seleccionado = (estacionSeleccionada == i);
            if (ImGui::Selectable(opcion.c_str(), seleccionado))
                estacionSeleccionada = i;
            if (seleccionado)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

        //Actualizar destino con la estación seleccionada
        filaDst = app.estaciones[estacionSeleccionada].fila;
        colDst  = app.estaciones[estacionSeleccionada].columna;
    }

    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Agregar paquete", {200, 45})) {
        if (peso < 1) peso = 1;
        errorMsg[0] = '\0';

        if (peso < 1 || peso > 20) {
            snprintf(errorMsg, sizeof(errorMsg),
                     "Error: el peso debe ser entre 1 y 20 kg.");
        } else if (!app.mapaListo) {
            snprintf(errorMsg, sizeof(errorMsg),
                     "Error: carga el mapa primero en Configuracion.");
        } else {
            bool esEstacion = false;
            for (const auto& e : app.estaciones) {
                if (e.fila == filaDst && e.columna == colDst) {
                    esEstacion = true;
                    break;
                }
            }
            if (!esEstacion) {
                snprintf(errorMsg, sizeof(errorMsg),
                         "Error: (%d,%d) no es una estacion en el mapa.",
                         filaDst, colDst);
            } else {
                Entrega nueva;
                nueva.id      = app.nextId++;
                nueva.peso    = peso;
                nueva.destino = {filaDst, colDst};
                app.entregas.push_back(nueva);
                peso    = 1;
                filaDst = 0;
                colDst  = 0;
            }
        }
    }

    if (errorMsg[0] != '\0') {
        ImGui::Spacing();
        ImGui::TextColored({1.f, 0.4f, 0.4f, 1.f}, "%s", errorMsg);
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::TextColored({0.5f, 0.8f, 1.f, 1.f},
                       "Paquetes registrados (%zu):", app.entregas.size());
    ImGui::Spacing();

    if (app.entregas.empty()) {
        ImGui::TextDisabled("No hay paquetes registrados todavia.");
    } else {
        ImGui::BeginTable("tablaPaquetes", 4,
                          ImGuiTableFlags_Borders |
                          ImGuiTableFlags_RowBg   |
                          ImGuiTableFlags_ScrollY,
                          {0, 400});

        ImGui::TableSetupColumn("ID",      ImGuiTableColumnFlags_WidthFixed,  80);
        ImGui::TableSetupColumn("Peso",    ImGuiTableColumnFlags_WidthFixed, 120);
        ImGui::TableSetupColumn("Destino", ImGuiTableColumnFlags_WidthFixed, 150);
        ImGui::TableSetupColumn("Accion",  ImGuiTableColumnFlags_WidthFixed, 100);
        ImGui::TableHeadersRow();

        int eliminar = -1;
        for (int i = 0; i < (int)app.entregas.size(); i++) {
            const auto& e = app.entregas[i];
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", e.id);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%.0f kg", e.peso);
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("(%d, %d)", e.destino.fila, e.destino.columna);
            ImGui::TableSetColumnIndex(3);
            ImGui::PushID(i);
            if (ImGui::SmallButton("Eliminar")) eliminar = i;
            ImGui::PopID();
        }

        if (eliminar >= 0)
            app.entregas.erase(app.entregas.begin() + eliminar);

        ImGui::EndTable();
    }

    ImGui::Spacing();
    if (ImGui::Button("< Volver", {120, 40})) app.pantalla = 0;

    ImGui::End();
}

//Pantalla de configuración
void mostrarConfiguracion(AppState& app) {
    ImGui::SetNextWindowPos({300, 50}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({1580, 980}, ImGuiCond_Always);
    ImGui::Begin("Configuracion", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);

    // Carga mapa
    ImGui::TextColored({0.5f, 0.8f, 1.f, 1.f}, "Mapa JSON");
    ImGui::Separator();
    ImGui::Spacing();

    static char rutaJson[512] = "data/tablero.json";
    ImGui::Text("Ruta del archivo JSON:");
    ImGui::SetNextItemWidth(600);
    ImGui::InputText("##ruta", rutaJson, sizeof(rutaJson));
    ImGui::SameLine();

    if (ImGui::Button("Cargar mapa", {140, 30})) {
        app.tablero.assign(20, std::vector<int>(10, 0));
        app.estaciones.clear();
        app.inicio             = {-1, -1};
        app.mapaListo          = false;
        app.rutaCalculadaLista = false;
        app.viajes.clear();
        app.caminoCompleto.clear();
        app.viajeActual        = 0;
        cargarMapaDesdeJSON(rutaJson, app.tablero, app.inicio, app.estaciones);
        app.mapaListo = (app.inicio.fila != -1);

        std::cout << "[DEBUG] inicio real: " << app.inicio.fila << "," << app.inicio.columna << "\n";
        std::cout << "[DEBUG] tablero[0][0]: " << app.tablero[0][0] << "\n";
        std::cout << "[DEBUG] tablero[0][1]: " << app.tablero[0][1] << "\n";
        std::cout << "[DEBUG] tablero[1][0]: " << app.tablero[1][0] << "\n";
        std::cout << "[DEBUG] tablero[1][1]: " << app.tablero[1][1] << "\n";
        std::cout << "[DEBUG] estaciones size: " << app.estaciones.size() << "\n";
        // Probar BFS desde inicio a primera estacion
        if (!app.estaciones.empty()) {
            int dist = calcularDistanciaBFSRuta(app.tablero, app.inicio, app.estaciones[0]);
            std::cout << "[DEBUG] BFS inicio->estacion[0]: " << dist << "\n";
            std::cout << "[DEBUG] estacion[0]: " << app.estaciones[0].fila << "," << app.estaciones[0].columna << "\n";
        
        std::cout << "[DEBUG] fila 0: ";
        for (int j = 0; j < (int)app.tablero[0].size(); j++)
            std::cout << app.tablero[0][j] << " ";
        std::cout << "\n";

        std::cout << "[DEBUG] Estaciones alcanzables:\n";
        for (int i = 0; i < (int)app.estaciones.size(); i++) {
            int dist = calcularDistanciaBFSRuta(app.tablero, app.inicio, app.estaciones[i]);
            std::cout << "  estacion[" << i << "] (" 
                    << app.estaciones[i].fila << "," 
                    << app.estaciones[i].columna << ") -> dist: " << dist << "\n";
}
}
    }

    ImGui::Spacing();
    if (app.mapaListo) {
        ImGui::TextColored({0.4f, 1.f, 0.4f, 1.f},
                           "Mapa cargado  |  Inicio: (%d,%d)  |  Estaciones: %zu",
                           app.inicio.fila, app.inicio.columna,
                           app.estaciones.size());
    } else {
        ImGui::TextColored({1.f, 0.6f, 0.2f, 1.f}, "Mapa no cargado.");
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ── Max deliveries ────────────────────────────────────────
    ImGui::TextColored({0.5f, 0.8f, 1.f, 1.f}, "Entregas por viaje");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("max_deliveries_per_trip:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("##maxdel", &app.maxDeliveries);
    if (app.maxDeliveries < 1) app.maxDeliveries = 1;

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ── Algoritmos ────────────────────────────────────────────
    ImGui::TextColored({0.5f, 0.8f, 1.f, 1.f}, "Algoritmos a ejecutar");
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::RadioButton("Programacion Dinamica",    app.algoSeleccionado == 0)) app.algoSeleccionado = 0;
    if (ImGui::RadioButton("Algoritmos Geneticos",     app.algoSeleccionado == 1)) app.algoSeleccionado = 1;
    if (ImGui::RadioButton("Greedy",app.algoSeleccionado == 2)) app.algoSeleccionado = 2;
    if (ImGui::RadioButton("Backtracking",             app.algoSeleccionado == 3)) app.algoSeleccionado = 3;
    if (ImGui::RadioButton("Divide y Venceras",        app.algoSeleccionado == 4)) app.algoSeleccionado = 4;
    if (ImGui::RadioButton("Monte Carlo / Las Vegas",  app.algoSeleccionado == 5)) app.algoSeleccionado = 5;
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("< Volver", {120, 40})) app.pantalla = 0;

    ImGui::End();
}

//Pantalla de preview de ruta
void mostrarPreview(AppState& app) {
    ImGui::SetNextWindowPos({300, 50}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({1580, 980}, ImGuiCond_Always);
    ImGui::Begin("Preview de Ruta", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);

    ImGui::TextColored({0.5f, 0.8f, 1.f, 1.f}, "Vista previa del mapa");
    ImGui::Separator();
    ImGui::Spacing();

    if (!app.mapaListo) {
        ImGui::TextColored({1.f, 0.6f, 0.2f, 1.f},
                           "Carga el mapa primero en Configuracion.");
        if (ImGui::Button("< Volver", {120, 40})) app.pantalla = 0;
        ImGui::End();
        return;
    }

    //Botón para calcular ruta
    if (app.entregas.empty()) {
        ImGui::TextColored({1.f, 0.6f, 0.2f, 1.f},
                           "Registra paquetes primero.");
    } else {
        const char* nombresAlgo[] = {
            "Programacion Dinamica", "Algoritmo Genetico",
            "Greedy", "Backtracking",
            "Divide y Venceras", "Monte Carlo / Las Vegas"
        };
        ImGui::Text("Algoritmo: %s", nombresAlgo[app.algoSeleccionado]);
        ImGui::SameLine();

        if (ImGui::Button("Calcular Ruta", {150, 30})) {
            app.viajes.clear();
            app.rutaCalculadaLista = false;
            app.viajeActual = 0;

            // Dividir entregas en grupos de maxDeliveries
            std::vector<Entrega> pendientes = app.entregas;
    
            while (!pendientes.empty()) {
                // Tomar hasta maxDeliveries paquetes para este viaje
                std::vector<Entrega> grupo;
                int tomados = 0;
                float pesoGrupo = 0.f;

                for (int i = 0; i < (int)pendientes.size() && tomados < app.maxDeliveries; i++) {
                    if (pesoGrupo + pendientes[i].peso <= 20.f) {
                        grupo.push_back(pendientes[i]);
                        pesoGrupo += pendientes[i].peso;
                        tomados++;
                    }
                }

                //Eliminar los tomados de pendientes
                for (const auto& g : grupo) {
                    pendientes.erase(std::remove_if(pendientes.begin(), pendientes.end(),
                        [&](const Entrega& e){ return e.id == g.id; }), pendientes.end());
                }

                //Calcular ruta para este grupo
                ResultadoRuta ruta;
                switch (app.algoSeleccionado) {
                    case 0: ruta = algoritmoProgramacionDinamica(app.tablero, app.inicio, grupo, 20.f); break;
                    case 1: ruta = algoritmoGenetico(app.tablero, app.inicio, grupo, 20.f, 50, 100); break;
                    case 2: ruta = algoritmoGreedy(app.tablero, app.inicio, grupo, 20.f); break;
                    case 3: ruta = algoritmoBacktracking(app.tablero, app.inicio, grupo, 20.f); break;
                    case 4: ruta = algoritmoGreedy(app.tablero, app.inicio, grupo, 20.f);
                            ruta.tecnica = "Divide y Venceras (ruta via Greedy)"; break;
                    case 5: ruta = algoritmoMonteCarlo(app.tablero, app.inicio, grupo, 1000, 20.f); break;
                }
                app.viajes.push_back(ruta);
            }

            app.rutaCalculadaLista = true;
            // Construir camino completo del viaje 0
            app.caminoCompleto.clear();
            if (app.rutaCalculada.rutaValida) {
                Punto actual = app.inicio;
                for (int seg = 0; seg < (int)app.rutaCalculada.ruta.size(); seg++) {
                    auto camino = obtenerCaminoBFS(app.tablero, actual, app.rutaCalculada.ruta[seg].destino);
                    for (int pi = 0; pi < (int)camino.size() - 1; pi++) {
                    const auto& p = camino[pi];
                        // Buscar si ya existe este punto
                        bool encontrado = false;
                        for (auto& par : app.caminoCompleto) {
                            if (par.first.fila == p.fila && par.first.columna == p.columna) {
                                par.second.push_back(seg + 1);
                                encontrado = true;
                                break;
                            }
                        }
                        if (!encontrado)
                            app.caminoCompleto.push_back({p, {seg + 1}});
                    }
                    actual = app.rutaCalculada.ruta[seg].destino;
                }
                // Regreso al inicio
                auto regreso = obtenerCaminoBFS(app.tablero, actual, app.inicio);
                for (const auto& p : regreso) {
                    bool encontrado = false;
                    for (auto& par : app.caminoCompleto) {
                        if (par.first.fila == p.fila && par.first.columna == p.columna) {
                            par.second.push_back(0); // 0 = regreso
                            encontrado = true;
                            break;
                        }
                    }
                    if (!encontrado)
                        app.caminoCompleto.push_back({p, {0}});
                }
            }
    
            app.rutaCalculada = app.viajes.empty() ? ResultadoRuta{} : app.viajes[0];
        }

       if (app.rutaCalculadaLista && app.viajes.size() > 1) {
            ImGui::Spacing();
            ImGui::Text("Viaje %d de %d", app.viajeActual + 1, (int)app.viajes.size());
            ImGui::SameLine();
            if (ImGui::Button("< Anterior", {100, 25})) {
                if (app.viajeActual > 0) {
                    app.viajeActual--;
                    app.rutaCalculada = app.viajes[app.viajeActual];
                    app.caminoCompleto.clear();
                    if (app.rutaCalculada.rutaValida) {
                        Punto actual = app.inicio;
                        for (int seg = 0; seg < (int)app.rutaCalculada.ruta.size(); seg++) {
                            auto camino = obtenerCaminoBFS(app.tablero, actual, app.rutaCalculada.ruta[seg].destino);
                            for (int pi = 0; pi < (int)camino.size() - 1; pi++) {
                            const auto& p = camino[pi];
                                // Buscar si ya existe este punto
                                bool encontrado = false;
                                for (auto& par : app.caminoCompleto) {
                                    if (par.first.fila == p.fila && par.first.columna == p.columna) {
                                        par.second.push_back(seg + 1);
                                        encontrado = true;
                                        break;
                                    }
                                }
                                if (!encontrado)
                                    app.caminoCompleto.push_back({p, {seg + 1}});
                            }
                            actual = app.rutaCalculada.ruta[seg].destino;
                        }
                        // Regreso al inicio
                        auto regreso = obtenerCaminoBFS(app.tablero, actual, app.inicio);
                        for (const auto& p : regreso) {
                            bool encontrado = false;
                            for (auto& par : app.caminoCompleto) {
                                if (par.first.fila == p.fila && par.first.columna == p.columna) {
                                    par.second.push_back(0); // 0 = regreso
                                    encontrado = true;
                                    break;
                                }
                            }
                            if (!encontrado)
                                app.caminoCompleto.push_back({p, {0}});
                        }
                    }
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Siguiente >", {100, 25})) {
                if (app.viajeActual < (int)app.viajes.size() - 1) {
                    app.viajeActual++;
                    app.rutaCalculada = app.viajes[app.viajeActual];
                    app.caminoCompleto.clear();
                    if (app.rutaCalculada.rutaValida) {
                        Punto actual = app.inicio;
                        for (int seg = 0; seg < (int)app.rutaCalculada.ruta.size(); seg++) {
                            auto camino = obtenerCaminoBFS(app.tablero, actual, app.rutaCalculada.ruta[seg].destino);
                            for (int pi = 0; pi < (int)camino.size() - 1; pi++) {
                            const auto& p = camino[pi];
                                // Buscar si ya existe este punto
                                bool encontrado = false;
                                for (auto& par : app.caminoCompleto) {
                                    if (par.first.fila == p.fila && par.first.columna == p.columna) {
                                        par.second.push_back(seg + 1);
                                        encontrado = true;
                                        break;
                                    }
                                }
                                if (!encontrado)
                                    app.caminoCompleto.push_back({p, {seg + 1}});
                            }
                            actual = app.rutaCalculada.ruta[seg].destino;
                        }
                        // Regreso al inicio
                        auto regreso = obtenerCaminoBFS(app.tablero, actual, app.inicio);
                        for (const auto& p : regreso) {
                            bool encontrado = false;
                            for (auto& par : app.caminoCompleto) {
                                if (par.first.fila == p.fila && par.first.columna == p.columna) {
                                    par.second.push_back(0); // 0 = regreso
                                    encontrado = true;
                                    break;
                                }
                            }
                            if (!encontrado)
                                app.caminoCompleto.push_back({p, {0}});
                        }
                    }
                }
            }
        }
    }
    ImGui::Spacing();
    if (app.rutaCalculadaLista) {
        int costoTotal = 0;
        for (const auto& v : app.viajes)
            costoTotal += v.costoTotal;
        ImGui::Text("Costo viaje actual: %d  |  Costo total todos los viajes: %d",
            app.rutaCalculada.costoTotal, costoTotal);
    }

    //Grid del mapa
    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2      base = ImGui::GetCursorScreenPos();
    const float celda = 40.f;

    // Construir set de destinos en la ruta para pintarlos
    std::vector<Punto> destinosRuta;
    if (app.rutaCalculadaLista && app.rutaCalculada.rutaValida) {
        for (const auto& e : app.rutaCalculada.ruta)
            destinosRuta.push_back(e.destino);
    }

    for (int i = 0; i < (int)app.tablero.size(); i++) {
        for (int j = 0; j < (int)app.tablero[i].size(); j++) {
            ImVec2 p0 = {base.x + j * celda, base.y + i * celda};
            ImVec2 p1 = {p0.x + celda - 2,   p0.y + celda - 2};

            ImU32 color;
            switch (app.tablero[i][j]) {
                case 1:  color = IM_COL32( 50, 100, 200, 255); break;
                case 2:  color = IM_COL32(230, 200,  30, 255); break;
                case 3:  color = IM_COL32(200,  50,  50, 255); break;
                default: color = IM_COL32( 30,  30,  40, 255); break;
            }


            // Verificar si es destino de la ruta
            int enRutaIdx = -1;
            for (int k = 0; k < (int)destinosRuta.size(); k++) {
                if (destinosRuta[k].fila == i && destinosRuta[k].columna == j) {
                    enRutaIdx = k;
                    break;
                }
            }

            // Verificar si es parte del camino recorrido
            std::vector<int> segmentosCamino;
            if (enRutaIdx < 0) {
                for (const auto& par : app.caminoCompleto) {
                    if (par.first.fila == i && par.first.columna == j) {
                        segmentosCamino = par.second;
                        break;
                    }
                }
            }

            if (enRutaIdx >= 0) {
                color = IM_COL32(50, 220, 100, 255);
                draw->AddRectFilled(p0, p1, color, 3.f);
                char buf[8];
                snprintf(buf, sizeof(buf), "%d", enRutaIdx + 1);
                draw->AddText({p0.x + 14, p0.y + 12}, IM_COL32(0,0,0,255), buf);
            } else if (!segmentosCamino.empty()) {
                draw->AddRectFilled(p0, p1, IM_COL32(100, 180, 255, 255), 3.f);
                std::string label = "";
                for (int s : segmentosCamino) {
                    if (s == 0) continue;
                    if (!label.empty()) label += "-";
                    label += std::to_string(s);
                }
                if (!label.empty())
                    draw->AddText({p0.x + 4, p0.y + 12}, IM_COL32(0,0,0,255), label.c_str());
            } else {
                draw->AddRectFilled(p0, p1, color, 3.f);
            }
        }
    }

    ImGui::Dummy({(float)app.tablero[0].size() * celda,
                  (float)app.tablero.size()    * celda});

    //Significado de colores
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();


    //Leyenda con cuadros de color
    ImDrawList* drawL = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float sz = 16.f;
    float gap = 8.f;

    drawL->AddRectFilled(pos, {pos.x+sz, pos.y+sz}, IM_COL32(230,200,30,255), 2.f);
    ImGui::Dummy({sz, sz}); ImGui::SameLine(0, gap);
    ImGui::Text("Inicio"); ImGui::SameLine(0, 20);

    pos = ImGui::GetCursorScreenPos();
    drawL->AddRectFilled(pos, {pos.x+sz, pos.y+sz}, IM_COL32(50,100,200,255), 2.f);
    ImGui::Dummy({sz, sz}); ImGui::SameLine(0, gap);
    ImGui::Text("Camino"); ImGui::SameLine(0, 20);

    pos = ImGui::GetCursorScreenPos();
    drawL->AddRectFilled(pos, {pos.x+sz, pos.y+sz}, IM_COL32(200,50,50,255), 2.f);
    ImGui::Dummy({sz, sz}); ImGui::SameLine(0, gap);
    ImGui::Text("Estacion"); ImGui::SameLine(0, 20);

    pos = ImGui::GetCursorScreenPos();
    drawL->AddRectFilled(pos, {pos.x+sz, pos.y+sz}, IM_COL32(100,180,255,255), 2.f);
    ImGui::Dummy({sz, sz}); ImGui::SameLine(0, gap);
    ImGui::Text("Recorrido"); ImGui::SameLine(0, 20);

    pos = ImGui::GetCursorScreenPos();
    drawL->AddRectFilled(pos, {pos.x+sz, pos.y+sz}, IM_COL32(50,220,100,255), 2.f);
    ImGui::Dummy({sz, sz}); ImGui::SameLine(0, gap);
    ImGui::Text("En ruta");

    ImGui::Spacing();
    if (ImGui::Button("< Volver", {120, 40})) app.pantalla = 0;

    ImGui::End();
}


//Pantalla de enviar ruta al robot
void mostrarEnviar(AppState& app) {
    ImGui::SetNextWindowPos({300, 50}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({1580, 980}, ImGuiCond_Always);
    ImGui::Begin("Enviar al Robot", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);

    ImGui::TextColored({0.5f, 0.8f, 1.f, 1.f}, "Enviar instrucciones al robot");
    ImGui::Separator();
    ImGui::Spacing();

    // ── Variables estáticas ───────────────────────────────────
    static char puertoCOM[16] = "COM8";
    static HANDLE hSerial = INVALID_HANDLE_VALUE;
    static bool conectado = false;
    static char mensajeConexion[128] = "";
    static HANDLE hHiloKeepAlive = INVALID_HANDLE_VALUE;

    // ── Conexión Bluetooth ────────────────────────────────────
    ImGui::Text("Puerto COM:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    ImGui::InputText("##com", puertoCOM, sizeof(puertoCOM));
    ImGui::SameLine();

    if (!conectado) {
        if (ImGui::Button("Conectar", {100, 30})) {
            std::string puerto = "\\\\.\\" + std::string(puertoCOM);
            hSerial = CreateFileA(puerto.c_str(),
                GENERIC_READ | GENERIC_WRITE, 0, nullptr,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

            if (hSerial == INVALID_HANDLE_VALUE) {
                snprintf(mensajeConexion, sizeof(mensajeConexion),
                    "Error: no se pudo abrir %s (cod: %lu)", puertoCOM, GetLastError());
                conectado = false;
            } else {
                DCB dcb = {0};
                dcb.DCBlength = sizeof(dcb);
                GetCommState(hSerial, &dcb);
                dcb.BaudRate = CBR_9600;
                dcb.ByteSize = 8;
                dcb.StopBits = ONESTOPBIT;
                dcb.Parity   = NOPARITY;
                SetCommState(hSerial, &dcb);

                COMMTIMEOUTS timeouts = {0};
                timeouts.ReadIntervalTimeout        = 10;
                timeouts.ReadTotalTimeoutConstant   = 10;
                timeouts.ReadTotalTimeoutMultiplier = 1;
                timeouts.WriteTotalTimeoutConstant  = 50;
                timeouts.WriteTotalTimeoutMultiplier= 10;
                SetCommTimeouts(hSerial, &timeouts);

                // Hilo keep-alive: lee cada 300ms para mantener el Bluetooth vivo
                hHiloKeepAlive = CreateThread(nullptr, 0,
                    [](LPVOID param) -> DWORD {
                        HANDLE h = (HANDLE)param;
                        char buf[1];
                        DWORD leido;
                        while (true) {
                            if (!ReadFile(h, buf, 1, &leido, nullptr)) {
                                if (GetLastError() == ERROR_INVALID_HANDLE) break;
                            }
                            Sleep(300);
                        }
                        return 0;
                    }, hSerial, 0, nullptr);

                snprintf(mensajeConexion, sizeof(mensajeConexion),
                    "Conectado a %s", puertoCOM);
                conectado = true;
            }
        }
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button, {0.6f, 0.1f, 0.1f, 1.f});
        if (ImGui::Button("Desconectar", {110, 30})) {
            // Cerrar handle hace que el hilo salga solo por ERROR_INVALID_HANDLE
            CloseHandle(hSerial);
            hSerial = INVALID_HANDLE_VALUE;
            if (hHiloKeepAlive != INVALID_HANDLE_VALUE) {
                WaitForSingleObject(hHiloKeepAlive, 1000);
                CloseHandle(hHiloKeepAlive);
                hHiloKeepAlive = INVALID_HANDLE_VALUE;
            }
            conectado = false;
            snprintf(mensajeConexion, sizeof(mensajeConexion), "Desconectado.");
        }
        ImGui::PopStyleColor();
    }

    if (mensajeConexion[0] != '\0') {
        ImGui::SameLine();
        if (conectado)
            ImGui::TextColored({0.4f, 1.f, 0.4f, 1.f}, "%s", mensajeConexion);
        else
            ImGui::TextColored({1.f, 0.4f, 0.4f, 1.f}, "%s", mensajeConexion);
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ── Estado ────────────────────────────────────────────────
    if (!app.rutaCalculadaLista) {
        ImGui::TextColored({1.f, 0.6f, 0.2f, 1.f},
            "Calcula la ruta primero en Preview.");
        if (ImGui::Button("< Volver", {120, 40})) app.pantalla = 0;
        ImGui::End();
        return;
    }

    ImGui::Text("Viajes calculados: %d", (int)app.viajes.size());
    ImGui::Text("Viaje actual: %d", app.viajeActual + 1);
    ImGui::Spacing();

    // ── Generar comandos ──────────────────────────────────────
    auto generarComandos = [&]() -> std::vector<char> {
        std::vector<char> cmds;
        if (app.caminoCompleto.empty()) return cmds;

        int orientacion = 0; // 0=Este 1=Sur 2=Oeste 3=Norte

        std::vector<Punto> recorrido;
        recorrido.push_back(app.inicio);
        for (const auto& e : app.rutaCalculada.ruta)
            recorrido.push_back(e.destino);
        recorrido.push_back(app.inicio);

        for (int paso = 0; paso + 1 < (int)recorrido.size(); paso++) {
            auto segmento = obtenerCaminoBFS(app.tablero,
                recorrido[paso], recorrido[paso + 1]);

            for (int k = 0; k + 1 < (int)segmento.size(); k++) {
                int df = segmento[k+1].fila    - segmento[k].fila;
                int dc = segmento[k+1].columna - segmento[k].columna;

                int dirDeseada = 0;
                if      (dc ==  1) dirDeseada = 0;
                else if (df ==  1) dirDeseada = 1;
                else if (dc == -1) dirDeseada = 2;
                else if (df == -1) dirDeseada = 3;

                int diff = (dirDeseada - orientacion + 4) % 4;
                if (diff == 1)      cmds.push_back('R');
                else if (diff == 2) { cmds.push_back('R'); cmds.push_back('R'); }
                else if (diff == 3) cmds.push_back('L');

                orientacion = dirDeseada;
                cmds.push_back('F');
            }

            if (paso < (int)recorrido.size() - 2)
                cmds.push_back('D');
        }
        return cmds;
    };

    auto comandos = generarComandos();

    // ── Preview de comandos ───────────────────────────────────
    ImGui::TextColored({0.5f, 0.8f, 1.f, 1.f}, "Comandos a enviar:");
    ImGui::Spacing();
    std::string preview = "";
    for (char c : comandos) preview += c;
    preview += "  (total: " + std::to_string(comandos.size()) + ")";
    ImGui::TextWrapped("%s", preview.c_str());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ── Botón enviar ──────────────────────────────────────────
    if (!conectado) {
        ImGui::TextColored({1.f, 0.6f, 0.2f, 1.f}, "Conecta el Bluetooth primero.");
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button,        {0.2f, 0.6f, 0.2f, 1.f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.3f, 0.8f, 0.3f, 1.f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  {0.1f, 0.4f, 0.1f, 1.f});

        if (ImGui::Button("ENVIAR AL ROBOT", {250, 60})) {
            for (char cmd : comandos) {
                std::string s(1, cmd);
                s += "\n";
                DWORD written;
                WriteFile(hSerial, s.c_str(), s.size(), &written, nullptr);
                Sleep(1000);
            }
            std::cout << "[Robot] Comandos enviados: " << comandos.size() << "\n";
        }

        ImGui::PopStyleColor(3);
    }

    ImGui::Spacing();
    if (ImGui::Button("< Volver", {120, 40})) app.pantalla = 0;

    ImGui::End();
}