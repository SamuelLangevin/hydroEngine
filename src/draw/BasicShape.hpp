#ifndef BASICSHAPE_HPP
#define BASICSHAPE_HPP
#include "Shape.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../utility/Utility.hpp"

class BasicShape : public Shape {
public:

    Utility::Material material;
    uint normalMap = 0;
    uint displacementMap = 0;
    bool enableNormalMapping = false;

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

        if (enableNormalMapping) {
            shader->setInt("normalMap", 1);
            shader->setInt("displacementMap", 2);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normalMap);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, displacementMap);
        }
    }
};

#endif
