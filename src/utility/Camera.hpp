#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
    public:

        struct Plane {
            glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
            float distance = 0.0f;
            Plane() = default;
            Plane(const glm::vec3 & p1, const glm::vec3 & norm)
                : normal(glm::normalize(norm)), distance(glm::dot(normal, p1)){}

            [[nodiscard]] float getSignedDistanceToPlane(const glm::vec3& point) const
            {
                return glm::dot(normal, point) - distance;
            }
        };
        struct Frustum {
            Plane topFace;
            Plane bottomFace;
            Plane rightFace;
            Plane leftFace;
            Plane farFace;
            Plane nearFace;
        };

        Camera();

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        float zoom = 45.0f;
        const float nearClip = 0.1f;
        const float farClip = 1000.0f;

        [[nodiscard]] glm::mat4 getViewMatrix() const;
        [[nodiscard]] glm::mat4 getProjectionMatrix(glm::ivec2 windowSize) const;

        [[nodiscard]] Frustum createFrustumFromCamera(glm::ivec2 windowSize) const;
        [[nodiscard]] glm::vec3 screenClickToNearClip(glm::ivec2 clickPos, glm::ivec2 windowSize) const;

    private:;

};

#endif