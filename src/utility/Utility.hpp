#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "../../includes/glad.h"
#include <map>
#include <freetype/freetype.h>
#include <glm/glm.hpp>

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

        struct Character {
            uint textureID;
            glm::ivec2 size;
            glm::ivec2 bearing;
            uint advance;
        };
        typedef std::map<char, Character> Font;

        Utility() = delete;

        static void createFrameBuffer(FrameBuffer & FBO, glm::ivec2 windowSize, bool attachRenderBuffer, int colorAttachments);
        static void createGBuffer(GeometryFrameBuffer & FBO, glm::ivec2 windowSize);
        static void createMultiSampleFrameBuffer(FrameBuffer & FBO, glm::ivec2 windowSize);
        static void createDepthFrameBuffer(FrameBuffer & FBO, int shadowsResolution);
        static void createDepthCubeMapFrameBuffer(FrameBuffer & FBO, int shadowsResolution);
        static void createSSAOFRameBuffer(FrameBuffer & FBO, glm::ivec2 windowSize);
        static void createHDRCubemapFramebuffer(FrameBuffer & FBO, glm::ivec2 resolution);

        static GLenum glCheckError_(const char *file, int line);
        #define glCheckError() glCheckError_(__FILE__, __LINE__)

        static void loadFont(FT_Library & ft, Font & characters, const char * fontPath);

};

#endif
