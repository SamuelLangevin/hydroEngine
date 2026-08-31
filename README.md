# HydroEngine

## About
This project aims to create simple yet realistic wave physics for larges bodies of water, showcased in an interactive application.

## Setup (linux)

### Install
1.
```
sudo apt install cmake
sudo apt-get install libglfw-dev
```
2. Download glm from its [repository](https://github.com/g-truc/glm), then build and install.
3. Download assimp from its [repository](https://github.com/assimp/assimp), then build and install.

### glfw, glad/openGL setup
https://youtu.be/LxEFn-cGdE0?si=VZp4Sloh_Ba7o2I9

### Renderdoc setup
Renderdoc is an optional debugging tool.

1. Download renderdoc through its [website](https://renderdoc.org/).

2. run renderdoc as admin with command ```$sudo ./qrenderdoc```

This application uses ImGui as its graphical user interface, but does not require additional setup.

## Attributions
- [Scott Lembcke's blog Water Wave Simulation](https://www.slembcke.net/blog/WaterWaves/) was a great inspiration for the physics of this engine.
- [NVIDIA article's on water rendering](https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models) was very helpful.
- Some classes and functions of this application have been built from the book of [Joey de Vries](https://twitter.com/JoeyDeVriez), [LearnOpenGL](https://learnopengl.com/Introduction),
notably the implementation of [diffuse](https://learnopengl.com/PBR/IBL/Diffuse-irradiance) and [specular](https://learnopengl.com/PBR/IBL/Specular-IBL) IBL.
- The tessellation shader used for the water was based from [Dr. Jeffrey Paone's guest article on tessellation](https://learnopengl.com/Guest-Articles/2021/Tessellation/Tessellation)
- The frustum culling (not yet implemented) calculations come from [Six Jonathan's article](https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling).
- The [ImGui library](https://github.com/ocornut/imgui) was used for the graphical user interface.

## Used assets
- Lake skybox : https://github.com/JoeyDeVries/LearnOpenGL/tree/master/resources/textures/skybox

## Contributing
This is a personal project. You may suggest modifications, report bugs through issues but pull requests will not be merged.





