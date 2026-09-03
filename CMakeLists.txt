cmake_minimum_required(VERSION 3.11.0)
project(hydroEngine VERSION 1.0.0)
cmake_policy(SET CMP0135 NEW)
include(FetchContent)

find_package(OpenGL REQUIRED)

FetchContent_Declare(imgui_external
        URL https://github.com/ocornut/imgui/archive/refs/tags/v1.92.9b.tar.gz
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(imgui_external)

add_library(imgui
        ${imgui_external_SOURCE_DIR}/imgui.cpp
        ${imgui_external_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_external_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_external_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_external_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_external_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        ${imgui_external_SOURCE_DIR}
        ${imgui_external_SOURCE_DIR}/backends
)
add_executable(hydroEngine
    includes/glad.c
    includes/stb_image.cpp
    src/main.cpp
    src/Application.cpp
    src/appModules/SceneRenderer.cpp
    src/appModules/SceneManager.cpp
    src/appModules/GuiManager.cpp
    src/repositories/SceneRepository.cpp
    src/utility/Shader.cpp
    src/utility/Utility.cpp
    src/utility/Camera.cpp
    src/utility/ResourceManager.cpp
    src/utility/Texture.cpp
    src/utility/Waves.cpp
    src/draw/Sphere.cpp
    src/draw/Mesh.cpp
    src/draw/Rectangle.cpp
    src/draw/Surface.cpp
    src/draw/Cube.cpp
)

target_include_directories(hydroEngine PRIVATE /usr/local/include/glm)

target_link_libraries(hydroEngine
    imgui
    glfw
    OpenGL::GL
    assimp
    glm
)