#ifndef CONE_HPP
#define CONE_HPP
#include "BasicShape.hpp"

class Cone : public BasicShape{
public:

    Cone();
    static void free();
    static void load();
    void draw(const Shader & shader) const override;

    static int lonResolution;
private:
    static uint VAO, VBO, EBO;
};


#endif
