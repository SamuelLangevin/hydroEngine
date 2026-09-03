#include "Utility.hpp"
#include <iostream>
#include "Texture.hpp"

//todo refactor duplicated code : texture and renderbuffer creation

void Utility::createMultiSampleFrameBuffer(FrameBuffer & FBO, glm::ivec2 windowSize) {
    glGenFramebuffers(1, &FBO.ID);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO.ID);

    auto generateTextureBuffer = [&](uint & textureBufferId, const GLuint number) {
        glGenTextures(1, &textureBufferId);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureBufferId);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA16F, windowSize.x, windowSize.y, GL_TRUE);
        Texture::setParameters(GL_TEXTURE_2D, GL_CLAMP_TO_BORDER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, number, GL_TEXTURE_2D_MULTISAMPLE, textureBufferId, 0);
    };
    generateTextureBuffer(FBO.buffers[0], GL_COLOR_ATTACHMENT0);
    generateTextureBuffer(FBO.buffers[1], GL_COLOR_ATTACHMENT1);

    glGenRenderbuffers(1, &FBO.renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, FBO.renderBuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, windowSize.x, windowSize.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, FBO.renderBuffer);

    uint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Multisample framebuffer is not complete!\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Utility::createFrameBuffer(FrameBuffer & FBO, glm::ivec2 windowSize, bool attachRenderBuffer, int colorAttachments) {
    glGenFramebuffers(1, &FBO.ID);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO.ID);

    for (int i = 0; i < colorAttachments && i < 4; i++) {
        FBO.buffers[i] = Texture::createTexture(windowSize, GL_RGBA16F, GL_RGBA, GL_UNSIGNED_BYTE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, FBO.buffers[i], 0);
    }

    if (attachRenderBuffer) {
        glGenRenderbuffers(1, &FBO.renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, FBO.renderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, windowSize.x, windowSize.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, FBO.renderBuffer);
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

GLenum Utility::glCheckError_(const char *file, int line) {

    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            default: error = "UNKNOWN_ERROR"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << "\n";
    }
    return errorCode;
}

glm::vec3 Utility::getIntersectionOfLinePlane(const glm::vec3 lineP1, const glm::vec3 lineP2,
                                    const glm::vec3 planePosition, const glm::vec3 planeNormal) {
    constexpr float eps = 0.00001;
    glm::vec3 line = lineP2 - lineP1;
    float dot = glm::dot(planeNormal, line);

    if (std::abs(dot) > eps) {
        const glm::vec3 w = lineP1 - planePosition;
        float travelFactor = -(glm::dot(planeNormal, w)) / dot;
        return travelFactor * line + lineP1;
    }

    return glm::vec3(0.0f); // No intersection
}

glm::vec3 Utility::getClickPositionOnPlane(const glm::ivec2 clickPos, const Camera & camera,
                const glm::vec3 planePosition, const glm::vec3 planeNormal, const glm::ivec2 windowSize) {
    const glm::vec3 p1 = camera.screenClickToNearClip(clickPos, windowSize);
    return getIntersectionOfLinePlane(camera.getPosition(), p1, planePosition, planeNormal);
}


