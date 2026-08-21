#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "../../includes/glad.h"
#include <glm/glm.hpp>

#include "Camera.hpp"

class Utility{
    public:

        struct Material {
            uint texture_diffuse0 = 0;
            float metallic = 0.5f;
            float roughness = 0.5f;
            float ao = 1.0f;
        };

        struct FrameBuffer {
            uint ID{};
            uint textureBuffer0{};
            uint textureBuffer1{};
            uint textureBuffer2{};
            uint renderBuffer{};
        };

        struct GeometryFrameBuffer {
            uint ID{};
            uint positionTexture{};
            uint normalTexture{};
            uint albedoSpecTex{};
            uint renderBuffer{};
        };

        Utility() = delete;

        static void createFrameBuffer(FrameBuffer & FBO, glm::ivec2 windowSize, bool attachRenderBuffer, int colorAttachments);
        static void createGBuffer(GeometryFrameBuffer & FBO, glm::ivec2 windowSize);
        static void createMultiSampleFrameBuffer(FrameBuffer & FBO, glm::ivec2 windowSize);
        static void createDepthFrameBuffer(FrameBuffer & FBO, int shadowsResolution);
        static void createDepthCubeMapFrameBuffer(FrameBuffer & FBO, int shadowsResolution);
        static void createSSAOFrameBuffer(FrameBuffer & FBO, glm::ivec2 windowSize);
        static void createHDRCubemapFramebuffer(FrameBuffer & FBO, glm::ivec2 resolution);

        static GLenum glCheckError_(const char *file, int line);
        #define glCheckError() glCheckError_(__FILE__, __LINE__)

        static glm::vec3 getIntersectionOfLinePlane(glm::vec3 lineP0, glm::vec3 p1, glm::vec3 planePosition, glm::vec3 planeNormal);
        static glm::vec3 getClickPositionOnPlane(glm::ivec2 clickPos, const Camera & camera, glm::vec3 planePosition, glm::vec3 planeNormal, glm::ivec2 windowSize);


};

#endif
