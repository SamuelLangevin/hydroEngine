#ifndef CONE_HPP
#define CONE_HPP
#include "BasicShape.hpp"

class Cone : public BasicShape{
public:
    static uint VAO;

    Cone();
    static void load();
    void draw(Shader * shader) const override;

    static int lonResolution;
};


#endif
