#ifndef BASIC_SHAPE_HPP
#define BASIC_SHAPE_HPP
#include "Shape.hpp"
#include "../../resources/Texture.hpp"
#include "../../../includes/glad.h"

/** \class BasicShape
 * Base of a shape class with a material
 */
class BasicShape : public Shape {
public:

    /** PBR material for light computation. */
    struct Material {
        Texture texture_diffuse0 {0,glm::ivec2(0),GL_TEXTURE_2D}; /**< AKA albedo */
        float metallic = 0.0f; /** Proportion of specular reflection [0, 1]. */
        float roughness = 0.5f; /** Reverse of smoothness*/
        float ao = 1.0f; /**< Ambient occlusion. Factor of self occlusion. */
    };

    Material material;

    /**
     * Sets the model matrix uniform and the material uniforms.
     * @param shader
     * @param channel
     */
    void setUniforms(const Shader & shader, int channel) const {
        setMatricesUniforms(shader);

        shader.setFloat("material.metallic", material.metallic);
        shader.setFloat("material.roughness", material.roughness);
        shader.setFloat("material.ao", material.ao);
        material.texture_diffuse0.bind(shader, "material.texture_diffuse0", channel);

    }
};

#endif
