//Menu.cpp
#include <algorithm>
#include "../Include/Menu.h"
#include "../Include/RutasUtil.h"
#include <iostream>
#include <cstdio>

// Forward declaration de ConfMapa.cpp
void cargarMapaDesdeJSON(const std::string& ruta,
                         std::vector<std::vector<int>>& tablero,
                         Punto& inicio,
                         std::vector<Punto>& estaciones);

// ════════════════════════════════════════════════════════════════
//  INICIALIZACIÓN
// ════════════════════════════════════════════════════════════════
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

// ════════════════════════════════════════════════════════════════
//  LOOP PRINCIPAL
// ════════════════════════════════════════════════════════════════
void loopPrincipal(GLFWwindow* window, AppState& app) {
    ImGuiIO& io = ImGui::GetIO();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Fondo completo
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

        // Panel lateral siempre visible
        mostrarMenuPrincipal(app);

        // Pantalla activa
        switch (app.pantalla) {
            case 1: mostrarRegistro(app);      break;
            case 2: mostrarConfiguracion(app); break;
            case 3: mostrarPreview(app);        break;
            case 4: mostrarEnviar(app);         break;
        }

        // Render
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

// ════════════════════════════════════════════════════════════════
//  CLEANUP
// ════════════════════════════════════════════════════════════════
void limpiar(GLFWwindow* window) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

// ════════════════════════════════════════════════════════════════
//  PANTALLA 0 — Menú principal (panel lateral)
// ════════════════════════════════════════════════════════════════
void mostrarMenuPrincipal(AppState& app) {
    // ── Panel lateral ─────────────────────────────────────────
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

    // ── Área de bienvenida (solo cuando pantalla == 0) ────────
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

// ════════════════════════════════════════════════════════════════
//  PANTALLA 1 — Registro de paquetes
// ════════════════════════════════════════════════════════════════
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

        // Actualizar destino con la estación seleccionada
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

// ════════════════════════════════════════════════════════════════
//  PANTALLA 2 — Configuración
// ════════════════════════════════════════════════════════════════
void mostrarConfiguracion(AppState& app) {
    ImGui::SetNextWindowPos({300, 50}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({1580, 980}, ImGuiCond_Always);
    ImGui::Begin("Configuracion", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);

    // ── Cargar mapa ───────────────────────────────────────────
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
        app.inicio    = {-1, -1};
        app.mapaListo = false;
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

// ════════════════════════════════════════════════════════════════
//  PANTALLA 3 — Preview de ruta
// ════════════════════════════════════════════════════════════════
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

    // ── Botón calcular ruta ───────────────────────────────────
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

                // Eliminar los tomados de pendientes
                for (const auto& g : grupo) {
                    pendientes.erase(std::remove_if(pendientes.begin(), pendientes.end(),
                        [&](const Entrega& e){ return e.id == g.id; }), pendientes.end());
                }

                // Calcular ruta para este grupo
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
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Siguiente >", {100, 25})) {
                if (app.viajeActual < (int)app.viajes.size() - 1) {
                    app.viajeActual++;
                    app.rutaCalculada = app.viajes[app.viajeActual];
                }
            }
        }
    }
    ImGui::Spacing();

    // ── Grid del mapa ─────────────────────────────────────────
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


            int enRutaIdx = -1;
            for (int k = 0; k < (int)destinosRuta.size(); k++) {
                if (destinosRuta[k].fila == i && destinosRuta[k].columna == j) {
                    enRutaIdx = k;
                    break;
                }
            }
            if (enRutaIdx >= 0) {
                color = IM_COL32(50, 220, 100, 255);
                draw->AddRectFilled(p0, p1, color, 3.f);
                char buf[8];
                snprintf(buf, sizeof(buf), "%d", enRutaIdx + 1);
                draw->AddText({p0.x + 14, p0.y + 12}, IM_COL32(0,0,0,255), buf);
            } else {
                draw->AddRectFilled(p0, p1, color, 3.f);
            }
        }
    }

    ImGui::Dummy({(float)app.tablero[0].size() * celda,
                  (float)app.tablero.size()    * celda});

    // ── Leyenda ───────────────────────────────────────────────
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextColored({0.9f, 0.8f, 0.1f, 1.f}, "■"); ImGui::SameLine();
    ImGui::Text("Inicio");                              ImGui::SameLine();
    ImGui::Dummy({10, 0});                              ImGui::SameLine();
    ImGui::TextColored({0.2f, 0.4f, 0.8f, 1.f}, "■"); ImGui::SameLine();
    ImGui::Text("Camino");                              ImGui::SameLine();
    ImGui::Dummy({10, 0});                              ImGui::SameLine();
    ImGui::TextColored({0.8f, 0.2f, 0.2f, 1.f}, "■"); ImGui::SameLine();
    ImGui::Text("Estacion");                            ImGui::SameLine();
    ImGui::Dummy({10, 0});                              ImGui::SameLine();
    ImGui::TextColored({0.2f, 0.9f, 0.4f, 1.f}, "■"); ImGui::SameLine();
    ImGui::Text("En ruta");

    ImGui::Spacing();
    if (ImGui::Button("< Volver", {120, 40})) app.pantalla = 0;

    ImGui::End();
}


// ════════════════════════════════════════════════════════════════
//  PANTALLA 4 — Enviar al robot
// ════════════════════════════════════════════════════════════════
void mostrarEnviar(AppState& app) {
    ImGui::SetNextWindowPos({300, 50}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({1580, 980}, ImGuiCond_Always);
    ImGui::Begin("Enviar al Robot", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);

    ImGui::TextColored({0.5f, 0.8f, 1.f, 1.f}, "Instrucciones para el robot");
    ImGui::Separator();
    ImGui::Spacing();

    if (app.entregas.empty()) {
        ImGui::TextColored({1.f, 0.6f, 0.2f, 1.f},
                           "No hay paquetes registrados.");
    } else {
        ImGui::Text("Paquetes a entregar: %zu", app.entregas.size());
        ImGui::Text("Max entregas por viaje: %d", app.maxDeliveries);
        ImGui::Spacing();
        ImGui::TextDisabled(
            "(Las instrucciones se generaran cuando los algoritmos esten integrados)");
        ImGui::Spacing();

        ImGui::PushStyleColor(ImGuiCol_Button,        {0.2f, 0.6f, 0.2f, 1.f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.3f, 0.8f, 0.3f, 1.f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  {0.1f, 0.4f, 0.1f, 1.f});

        if (ImGui::Button("ENVIAR AL ROBOT", {250, 60})) {
            // TODO: conectar con comunicación serial
            std::cout << "[Robot] Enviando instrucciones...\n";
        }

        ImGui::PopStyleColor(3);
    }

    ImGui::Spacing();
    if (ImGui::Button("< Volver", {120, 40})) app.pantalla = 0;

    ImGui::End();
}