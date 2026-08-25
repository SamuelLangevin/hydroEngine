#ifndef BASIC_SHAPE_HPP
#define BASIC_SHAPE_HPP
#include "Shape.hpp"
#include "../utility/Texture.hpp"
#include "../../includes/glad.h"

class BasicShape : public Shape {
public:

    struct Material {
        Texture texture_diffuse0 {0,glm::ivec2(0),GL_TEXTURE_2D};
        float metallic = 0.5f;
        float roughness = 0.5f;
        float ao = 1.0f;
    };

    Material material;

    void setUniforms(const Shader & shader, int channel) const {
        setMatricesUniforms(shader);

        shader.setFloat("material.metallic", material.metallic);
        shader.setFloat("material.roughness", material.roughness);
        shader.setFloat("material.ao", material.ao);
        material.texture_diffuse0.bind(shader, "material.texture_diffuse0", channel);

    }
};

#endif
