cmake_minimum_required(VERSION 3.28)
project(heightMapper)

set(CMAKE_CXX_STANDARD 17)

# Find OpenGL
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(heightMapper
        src/main.cpp
        src/Core/Window.h
        src/Core/Window.cpp
        src/Core/Shader.h
        src/Core/Shader.cpp
        src/Core/Mesh.cpp
        src/Core/Mesh.h
        src/Game.cpp
        src/Game.h
        src/Scene.cpp
        src/Scene.h
        src/Core/Input.cpp
        src/Core/Input.h
        src/Events/Event.h
        src/Events/EventManager.cpp
        src/Events/EventManager.h
        src/Core/Camera.cpp
        src/Core/Camera.h
        src/Core/stb.cpp
        src/Core/Texture.cpp
        src/Core/Texture.h
        dependencies/include/fastNoise/FastNoise.cpp
        dependencies/include/fastNoise/FastNoise.h
        dependencies/include/ImGui/imgui.cpp
        dependencies/include/ImGui/imgui_draw.cpp
        dependencies/include/ImGui/imgui_demo.cpp
        dependencies/include/ImGui/imgui_tables.cpp
        dependencies/include/ImGui/imgui_widgets.cpp
        dependencies/include/ImGui/imgui_impl_opengl3.cpp
        dependencies/include/ImGui/imgui_impl_glfw.cpp
        dependencies/include/ImGui/imgui_impl_opengl3_loader.h
        src/Core/Gui.cpp
        src/Core/Gui.h
        src/Erosion/Erosion.cpp
        src/Erosion/Erosion.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include/ImGui)

# Collect all library files in the directory
file(GLOB LIB_FILES "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/library/*.dylib")

target_link_libraries(heightMapper ${LIB_FILES} ${OPENGL_LIBRARIES} GLEW::GLEW)
