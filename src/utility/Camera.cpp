#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()= default;

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(glm::ivec2 windowSize) const {
    return glm::perspective(glm::radians(zoom),
        static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), nearClip, farClip);
}

Camera::Frustum Camera::createFrustumFromCamera(glm::ivec2 windowSize) const {
    Frustum frustum;

    const float aspect = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    const float halfFarClipHoriz = farClip * tanf(zoom * 0.5f);
    const float halfFarClipVert = halfFarClipHoriz * aspect;
    const glm::vec3 frontMultFar = farClip * front;
    const glm::vec3 camRight = glm::cross(front, up);
    frustum.nearFace = {position + nearClip * front, front};
    frustum.farFace = {position + frontMultFar, -front};
    frustum.rightFace = {position, glm::cross(frontMultFar - camRight * halfFarClipHoriz, up)};
    frustum.leftFace = {position, glm::cross(up, frontMultFar + camRight * halfFarClipHoriz)};
    frustum.topFace = {position, glm::cross(camRight, frontMultFar - up * halfFarClipVert)};
    frustum.bottomFace = {position,  glm::cross(frontMultFar + up * halfFarClipVert, camRight)};

    return frustum;
}
