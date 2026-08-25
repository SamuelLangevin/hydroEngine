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
        void draw(const Shader & shader) const override;
        static void drawSkyBox(const Shader & shader, const Texture & skyBoxTex, const std::string & var);
};

#endif