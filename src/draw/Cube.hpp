#ifndef CUBE_H
#define CUBE_H

#include "../utility/Shader.hpp"
#include "BasicShape.hpp"


class Cube : public BasicShape {
    public:
        static uint cubeVAO;

        Cube();
        static void load();
        static void draw();
        void draw(Shader * shader) const override;
        static void drawSkyBox(Shader * shader, uint textureID, const std::string & var);
};

#endif