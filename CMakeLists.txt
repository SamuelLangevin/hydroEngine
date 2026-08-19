cmake_minimum_required(VERSION 3.11.0)
project(hydroEngine VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

add_executable(hydroEngine
    src/main.cpp
    includes/glad.c
    includes/stb_image.cpp
    src/utility/Shader.cpp
    src/SceneRenderer.cpp
    src/utility/Utility.cpp
    src/utility/Lights.cpp
    src/utility/Camera.cpp
    src/Application.cpp
    src/utility/ResourceManager.cpp
    src/utility/Texture.cpp
    src/draw/Cone.cpp
    src/draw/Sphere.cpp
    src/draw/Mesh.cpp
    src/draw/Rectangle.cpp
    src/utility/Waves.cpp
)

target_include_directories(hydroEngine PRIVATE /usr/local/include/freetype2)
target_include_directories(hydroEngine PRIVATE /usr/local/include/glm)

target_link_libraries(hydroEngine
    glfw
    OpenGL::GL
    assimp
    Freetype::Freetype
    glm
)