#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "BasicShape.hpp"

class Surface : public BasicShape{
public:

    explicit Surface(glm::ivec2 size);
    ~Surface() override;
    void draw(const Shader & shader) const override;


private:

    uint VAO = 0, VBO = 0;
    glm::ivec2 size;
    static uint resolution;
};


#endif
