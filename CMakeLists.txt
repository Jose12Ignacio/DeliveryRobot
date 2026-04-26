cmake_minimum_required(VERSION 3.15)
project(SmartRobot)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_INCLUDE "C:/glfw/include")
set(GLFW_LIB     "C:/glfw/lib-mingw-w64/libglfw3.a")

set(SOURCES
    menu/MenuMain.cpp
    menu/Menu.cpp
    pruebas/scr/Confimapa.cpp
    pruebas/scr/algortimos/Dividevenceras.cpp

    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(SmartRobot ${SOURCES})

target_include_directories(SmartRobot PRIVATE
    ${GLFW_INCLUDE}
    imgui
    imgui/backends
    menu
    Include
    pruebas/scr
    pruebas/scr/algortimos
)

target_link_libraries(SmartRobot PRIVATE
    ${GLFW_LIB}
    opengl32
    gdi32
)