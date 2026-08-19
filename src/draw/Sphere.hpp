#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "../utility/Shader.hpp"
#include "BasicShape.hpp"


class Sphere : public BasicShape {
public:
    static uint VAO;

    Sphere();
    static void load();
    void draw(Shader * shader) const override;

    static int latResolution, lonResolution;
};

#endif
