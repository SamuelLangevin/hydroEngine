#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "Shape.hpp"

class Surface : public Shape{
public:

    explicit Surface(glm::ivec2 size);
    ~Surface() override;
    void draw(Shader * shader) const override;


private:

    uint VAO = 0, VBO = 0;
    glm::ivec2 size;
    static uint resolution;
};


#endif
