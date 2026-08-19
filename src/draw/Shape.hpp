#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "../utility/Shader.hpp"
#include "../utility/Camera.hpp"

class Shape {
    public:

        struct BoundingSphere {
            glm::vec3 center = glm::vec3(0.0f);
            float radius = std::sqrt(2.0f);

            BoundingSphere() = default;
            BoundingSphere(const glm::vec3 & inCenter, float inRadius) : center(inCenter), radius(inRadius){}

            [[nodiscard]] bool isOnOrForwardPlane(const Camera::Plane& plane) const
            {
                return plane.getSignedDistanceToPlane(center) > -radius;
            }

            [[nodiscard]] bool isOnFrustum(const Camera::Frustum & frustum, const glm::mat4 & modelMatrix) const  {
                const glm::vec3 globalScale = glm::vec3(glm::length(modelMatrix[0]), glm::length(modelMatrix[1]), glm::length(modelMatrix[2]));
                const glm::vec3 globalCenter = modelMatrix * glm::vec4(center, 1.0f);
                const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);
                BoundingSphere globalSphere(globalCenter, radius * maxScale * 0.5f);
                
                return (globalSphere.isOnOrForwardPlane(frustum.leftFace) &&
                    globalSphere.isOnOrForwardPlane(frustum.rightFace) &&
                    globalSphere.isOnOrForwardPlane(frustum.farFace) &&
                    globalSphere.isOnOrForwardPlane(frustum.nearFace) &&
                    globalSphere.isOnOrForwardPlane(frustum.topFace) &&
                    globalSphere.isOnOrForwardPlane(frustum.bottomFace));
            }
        };

        glm::vec3 position = glm::vec3(0.0f);
        glm::quat orientation = glm::quat(0, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::vec3 scale = glm::vec3(1.0f);

        virtual ~Shape() = default;
        virtual void draw(Shader * shader) const = 0;
};

#endif