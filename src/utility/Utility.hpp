#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "../../includes/glad.h"
#include <glm/glm.hpp>

#include "Camera.hpp"

class Utility{
    public:

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

        /**
         * Prints the latest OpenGL error. Does not specify the error's location.
         * @param file the error was noticed in
         * @param line the line where the error was noticed.
         * @returns the error type
         */
        static GLenum glCheckError_(const char *file, int line);

        /** Prints the latest OpenGL error with the file and line where this macro has been called. */
        #define glCheckError() glCheckError_(__FILE__, __LINE__)

        /**
         * @param lineP1 first point of the line
         * @param lineP2 second point of the line
         * @param planePosition from the origin
         * @param planeNormal facing direction
         * @return the intersection point of the given line and plane.
         * If there is none, returns glm::vec3(0.0f)
         */
        static glm::vec3 getIntersectionOfLinePlane(glm::vec3 lineP1, glm::vec3 lineP2, glm::vec3 planePosition, glm::vec3 planeNormal);

        /**
         * @param clickPos
         * @param camera
         * @param planePosition
         * @param planeNormal
         * @param windowSize
         * @return the position on a given plane corresponding to a screen click
         */
        static glm::vec3 getClickPositionOnPlane(glm::ivec2 clickPos, const Camera & camera, glm::vec3 planePosition, glm::vec3 planeNormal, glm::ivec2 windowSize);


};

#endif
