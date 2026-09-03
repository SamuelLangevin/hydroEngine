#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "../../resources/Shader.hpp"
#include "../../scene/Camera.hpp"

/**
 * Entity class to draw something in the scene
 */
class Shape {
    public:

        /** \struct BoundingSphere
         * Base bounding volume to calculate the frustum culling.
         * Based on https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
         */
        struct BoundingSphere {
            glm::vec3 center = glm::vec3(0.0f); /**< The position of the enntity. */
            float radius = std::sqrt(2.0f); /**< The radius of the sphere that wraps the whole entity. */

            BoundingSphere() = default;
            BoundingSphere(const glm::vec3 & inCenter, float inRadius) : center(inCenter), radius(inRadius){}

            /**
             * Checks the distance to see if the volume is behind/through the plane or in front.
             * @param plane
             * @returns true if the plane cuts through part of the bounding sphere.
             */
            [[nodiscard]] bool isInFrontOfPlane(const Camera::Plane& plane) const
            {
                return plane.getSignedDistanceToPlane(center) > -radius;
            }

            /**
             * Generates the bounding sphere and checks if the volume is in front of each plane (inside the frustum).
             * @param frustum of the camera
             * @param modelMatrix of entity.
             * @returns true if the bounding volume is inside the camera's frustum.
             */
            [[nodiscard]] bool isOnFrustum(const Camera::Frustum & frustum, const glm::mat4 & modelMatrix) const  {
                const glm::vec3 globalScale = glm::vec3(glm::length(modelMatrix[0]), glm::length(modelMatrix[1]), glm::length(modelMatrix[2]));
                const glm::vec3 globalCenter = modelMatrix * glm::vec4(center, 1.0f);
                const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);
                BoundingSphere globalSphere(globalCenter, radius * maxScale * 0.5f);
                
                return (globalSphere.isInFrontOfPlane(frustum.leftFace) &&
                    globalSphere.isInFrontOfPlane(frustum.rightFace) &&
                    globalSphere.isInFrontOfPlane(frustum.farFace) &&
                    globalSphere.isInFrontOfPlane(frustum.nearFace) &&
                    globalSphere.isInFrontOfPlane(frustum.topFace) &&
                    globalSphere.isInFrontOfPlane(frustum.bottomFace));
            }
        };

        glm::vec3 position = glm::vec3(0.0f); /**< Relative position. */
        glm::quat orientation = glm::quat(0, glm::vec3(1.0f, 0.0f, 0.0f)); /**< Relative orientation /rotation. */
        glm::vec3 scale = glm::vec3(1.0f); /**< Relative size. */

        glm::vec3 waterDis = glm::vec3(0.0f);

        virtual ~Shape() = default;

        /**
         * Sets the model matrix of the shape.
         * @param shader to send the matrix to
         */
        void setMatricesUniforms(const Shader & shader) const {
            glm::mat4 model(1.0f);
            model = glm::translate(model, position + waterDis);
            model = glm::rotate(model, orientation.w, glm::vec3(orientation.x, orientation.y, orientation.z));
            model = glm::scale(model, scale);
            shader.setMat4("model", model);
        }

        virtual void draw(const Shader & shader) const = 0;
};

#endif