#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>


/** \class
 * Describes a scene's point of view.
 */
class Camera {
    public:

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

        glm::vec3 position = glm::vec3(0.0f, 10.0f, 40.0f); /**< The camera's position in the scene. */
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f); /**< Opposite direction to where the camera looks at. */
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); /**< The camera's absolute up direction. */
        float zoom = 45.0f; /**< The camera's vertical field of view angle, AKA fovY.*/
        const float nearClip = 0.1f; /**< The distance from the camera's position to its nearClip plane. */
        const float farClip = 1000.0f; /**< The distance from the camera's position to its farClip plane. */
        float speed = 20.0f; /**< Movement speed of the camera*/

};

#endif