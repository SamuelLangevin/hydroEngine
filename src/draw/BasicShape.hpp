#ifndef BASIC_SHAPE_HPP
#define BASIC_SHAPE_HPP
#include "Shape.hpp"
#include "../../includes/glad.h"

class BasicShape : public Shape {
public:

    struct Material {
        uint texture_diffuse0 = 0;
        float metallic = 0.5f;
        float roughness = 0.5f;
        float ao = 1.0f;
    };

    Material material;

    void setUniforms(Shader * shader, int channel) const {
        setMatricesUniforms(shader);

        shader->setFloat("material.metallic", material.metallic);
        shader->setFloat("material.roughness", material.roughness);
        shader->setFloat("material.ao", material.ao);
        shader->setInt("material.texture_diffuse0", channel);
        glActiveTexture(GL_TEXTURE0 + channel);
        glBindTexture(GL_TEXTURE_2D, material.texture_diffuse0);
    }
};

#endif
