#ifndef BASICSHAPE_HPP
#define BASICSHAPE_HPP
#include "Shape.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../utility/Utility.hpp"

class BasicShape : public Shape {
public:

    Utility::Material material;

protected:

    void setUniforms(Shader * shader) const {
        glm::mat4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, orientation.w, glm::vec3(orientation.x, orientation.y, orientation.z));
        model = glm::scale(model, scale);
        shader->setMat4("model", model);

        shader->setInt("material.texture_diffuse0", 0);
        shader->setFloat("material.metallic", material.metallic);
        shader->setFloat("material.roughness", material.roughness);
        shader->setFloat("material.ao", material.ao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.texture_diffuse0);
    }
};

#endif
