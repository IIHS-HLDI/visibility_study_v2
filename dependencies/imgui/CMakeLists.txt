
cmake_minimum_required(VERSION 3.10)

project(imgui VERSION 1.90.9)


add_library(imgui STATIC 
    ${PROJECT_SOURCE_DIR}/src/imgui.cpp
    ${PROJECT_SOURCE_DIR}/src/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/src/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/src/imgui_impl_opengl3.cpp
    ${PROJECT_SOURCE_DIR}/src/imgui_stdlib.cpp
    ${PROJECT_SOURCE_DIR}/src/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/src/imgui_widgets.cpp)

target_include_directories(imgui PRIVATE ${DEPENDENCY_DIR}/glfw3/include)

target_include_directories(imgui PUBLIC ${PROJECT_SOURCE_DIR}/include)
