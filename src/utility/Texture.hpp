#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP
#include <string>
#include <vec2.hpp>
#include <vec3.hpp>

#include "../../includes/glad.h"

class Texture{
public:

    uint ID = 0;
    GLenum type;

    Texture(uint textureID, GLenum type = GL_TEXTURE_2D);
    ~Texture();
    Texture(Texture & texture) = delete;
    void bind(GLenum textureChannel) const;

    static uint hdrTextureFromFile(const char * filenameChar, const std::string &directory);
    static uint textureFromFile(const char * filenameChar, const std::string &directory,
        GLint wrap = GL_CLAMP_TO_EDGE, GLint filters = GL_LINEAR);
    static uint createTexture( glm::ivec2 resolution, GLint internalFormat, GLenum format,
        GLenum type, GLint wrap, GLint filter, const void * data = nullptr);
    static uint createCubemapTexture(glm::ivec2 resolution);
    static uint cubemapFromDirectory(const std::string &directory);
    static uint createColorTexture(glm::vec3 color);

};

#endif