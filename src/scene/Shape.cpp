#include "Shape.hpp"

Shape::Shape(MeshRef mesh, TextureRef texture, const ShapeType type)
    : material{.texture_diffuse0 = texture}, mesh(mesh), shapeType(type) {}

void Shape::setUniforms(const Shader & shader) const {
    Entity::setUniforms(shader);

    shader.setFloat("material.metallic", material.metallic);
    shader.setFloat("material.roughness", material.roughness);
    shader.setFloat("material.ao", material.ao);
    material.texture_diffuse0.bind(shader, "material.texture_diffuse0", 0);

}

void Shape::draw(const Shader &shader) const {
    setUniforms(shader);
    mesh.draw();
}
