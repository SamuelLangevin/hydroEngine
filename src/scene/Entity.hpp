#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "../resources/Shader.hpp"

/**
 * Entity class to draw something in the scene
 */
class Entity {
    public:

        glm::vec3 position = glm::vec3(0.0f); /**< Relative position. */
        glm::quat orientation = glm::quat(0, glm::vec3(1.0f, 0.0f, 0.0f)); /**< Relative orientation /rotation. */
        glm::vec3 scale = glm::vec3(1.0f); /**< Relative size. */
        glm::vec3 waterDis = glm::vec3(0.0f);

        virtual ~Entity() = default;

        /**
         * Sets the model matrix of the shape.
         * @param shader to send the matrix to
         */
        virtual void setUniforms(const Shader & shader) const {
            glm::mat4 model(1.0f);
            model = glm::translate(model, position + waterDis);
            model = glm::rotate(model, orientation.w, glm::vec3(orientation.x, orientation.y, orientation.z));
            model = glm::scale(model, scale);
            shader.setMat4("model", model);
        }

        virtual void draw(const Shader & shader) const = 0;
};

#endif