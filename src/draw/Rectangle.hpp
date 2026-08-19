#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "BasicShape.hpp"


class Rectangle : public BasicShape {
public:
    static void draw2DQuad();
private:
    static uint screenQuadVAO;
};


#endif
