#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "../../includes/glad.h"
#include <glm/glm.hpp>

#include "Camera.hpp"

/** \class Utility
 * Holdall class to put useful functions.
 */
class Utility{
    public:
        /** \struct FrameBuffer
         * Stores the ID, the buffers IDs and the render buffer ID of a renderbuffer.
         */
        struct FrameBuffer {
            uint ID{};
            uint buffers[4]{};
            uint renderBuffer{};
        };

        Utility() = delete;

        /**
         * Generates a base framebuffers, and its attachments.
         * @param FBO object store the IDs in
         * @param windowSize the size of the framebuffer
         * @param attachRenderBuffer to generate and attach a render buffer
         * @param colorAttachments the number of textures to generate and attach
         */
        static void createFrameBuffer(FrameBuffer & FBO, glm::ivec2 windowSize, bool attachRenderBuffer, int colorAttachments);

        /**
         * Generates a multisample framebuffer for anti-aliasing
         * @param FBO object to store the IDs in
         * @param windowSize the size of the framebuffer
         */
        static void createMultiSampleFrameBuffer(FrameBuffer & FBO, glm::ivec2 windowSize);

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
