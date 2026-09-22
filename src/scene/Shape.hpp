#ifndef SHAPE_HPP
#define SHAPE_HPP
#include "Entity.hpp"
#include "../resources/ResourceRefs.hpp"


/** \class Shape
 * Base of a shape class with a material
 */
class Shape : public Entity {
public:

    /** PBR material for light computation. */
    struct Material {
        TextureRef texture_diffuse0; /**< AKA albedo */ //fixme rename to albedo
        float metallic = 0.0f; /** Proportion of specular reflection [0, 1]. */
        float roughness = 0.5f; /** Reverse of smoothness*/
        float ao = 1.0f; /**< Ambient occlusion. Factor of self occlusion. */
    };
    enum ShapeType {BOX, SPHERE, SURFACE, CUSTOM};

    Material material;
    MeshRef mesh;
    const ShapeType shapeType;

    Shape(MeshRef mesh, TextureRef texture, ShapeType type);

    /**
     * Sets the model matrix uniform and the material uniforms.
     * @param shader
     */
    void setUniforms(const Shader & shader) const override;

    void draw(const Shader & shader) const override;
};

#endif
