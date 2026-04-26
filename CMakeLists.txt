cmake_minimum_required(VERSION 3.15)
project(SmartRobot)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_INCLUDE "C:/glfw/include")
set(GLFW_LIB     "C:/glfw/lib-mingw-w64/libglfw3.a")

set(SOURCES
    Main.cpp
    Confimapa.cpp
    "Divide y venceras.cpp"
    menu/Menu.cpp
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
)

target_link_libraries(SmartRobot PRIVATE
    ${GLFW_LIB}
    opengl32
    gdi32
)