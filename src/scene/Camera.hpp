#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>


/** \class
 * Describes a scene's point of view.
 */
class Camera {
    public:
        /** \struct Plane
        * Describes a plane of the camera's frustum.
        */
        struct Plane {

            glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f); /**< A vector perpendicular to the plane. */
            float distance = 0.0f; /**< Distance from the origin to the nearest point on the plane*/

            Plane() = default;

            /**
             * Calculates its distance from the origin from the given parameters.
             * @param p1 A point on the plane
             * @param normal
             */
            Plane(const glm::vec3 & p1, const glm::vec3 & normal)
                : normal(glm::normalize(normal)), distance(glm::dot(normal, p1)){}

            /**
             * @param point
             * @returns the point's distance from the plane
             */
            [[nodiscard]] float getSignedDistanceToPlane(const glm::vec3& point) const
            {
                return glm::dot(normal, point) - distance;
            }
        };

        /** \struct Frustum
         * Used to describe the camera's frustum.
         */
        struct Frustum {
            Plane topFace; /**< The frustum's top delimitation. */
            Plane bottomFace; /**< The frustum's bottom delimitation. */
            Plane rightFace; /**< The frustum's right delimitation. */
            Plane leftFace; /**< The frustum's left delimitation. */
            Plane farFace; /**< The frustum's back delimitation. */
            Plane nearFace; /**< The frustum's front delimitation. */
        };

        Camera() = default;

        [[nodiscard]] glm::vec3 getPosition() const;

        /** @returns a matrix transforming to the camera's view space.*/
        [[nodiscard]] glm::mat4 getViewMatrix() const;

        /**
         * @param windowSize AKA viewport
         * @returns a matrix transforming to the screen's space.
         */
        [[nodiscard]] glm::mat4 getProjectionMatrix(glm::ivec2 windowSize) const;

        /**
         * @param windowSize AKA viewport
         * @returns the camera's frustum
         */
        [[nodiscard]] Frustum createFrustumFromCamera(glm::ivec2 windowSize) const;

        /**
         * @param clickPos on the screen
         * @param windowSize AKA viewport
         * @returns the world position on the nearClip of the camera corresponding to a screen click.
         */
        [[nodiscard]] glm::vec3 screenClickToNearClip(glm::ivec2 clickPos, glm::ivec2 windowSize) const;

        /**
         * Rotates the camera according the mouse offset.
         * @param offset the mouse's position difference
         */
        void rotate(glm::vec2 offset);

        /**
         * Moves the camera according to the direction booleans.
         * @param deltaTime the difference of time between the two frames
         * @param moveForward should the camera go forward ?
         * @param moveBack should the camera go backwards ?
         * @param moveLeft should the camera go to the left ?
         * @param moveRight should the camera go to the right ?
         */
        void move(float deltaTime, bool moveForward, bool moveBack, bool moveLeft, bool moveRight);

    private:;

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f); /**< The camera's position in the scene. */
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f); /**< Opposite direction to where the camera looks at. */
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); /**< The camera's absolute up direction. */
        float zoom = 45.0f; /**< The camera's vertical field of view angle, AKA fovY.*/
        const float nearClip = 0.1f; /**< The distance from the camera's position to its nearClip plane. */
        const float farClip = 1000.0f; /**< The distance from the camera's position to its farClip plane. */
        float speed = 20.0f; /**< Movement speed of the camera*/

};

#endif