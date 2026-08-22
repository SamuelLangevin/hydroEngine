#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

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

glm::vec3 Camera::screenClickToNearClip(const glm::ivec2 clickPos, const glm::ivec2 windowSize) const {
    const float halfHSide = nearClip * tanf(zoom * 0.5f);
    const float nearClipViewportRatio = halfHSide/(0.5f * windowSize.x);
    const glm::mat3 cameraSpace = glm::mat3(glm::cross(up, front), up, front);

    glm::vec3 screenCenterToClick = glm::vec3(windowSize.x/2.0f, windowSize.y/2.0f, 0.0f) - glm::vec3(clickPos, 0.0f);
    return position + front * nearClip + cameraSpace * screenCenterToClick *  nearClipViewportRatio;
}
