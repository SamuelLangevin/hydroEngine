#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "../utility/Shader.hpp"
#include "BasicShape.hpp"


class Sphere : public BasicShape {
public:

    Sphere();
    static void load();

    static void free();
    void draw(const Shader & shader) const override;

    static int latResolution, lonResolution;
private:
    static uint VAO, VBO, EBO;
};

#endif
