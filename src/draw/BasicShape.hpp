#ifndef BASIC_SHAPE_HPP
#define BASIC_SHAPE_HPP
#include "Shape.hpp"
#include "../utility/Utility.hpp"

class BasicShape : public Shape {
public:

    Utility::Material material;

protected:

    void setUniforms(Shader * shader) const {
        setMatricesUniforms(shader);

        shader->setInt("material.texture_diffuse0", 0);
        shader->setFloat("material.metallic", material.metallic);
        shader->setFloat("material.roughness", material.roughness);
        shader->setFloat("material.ao", material.ao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.texture_diffuse0);
    }
};

#endif
