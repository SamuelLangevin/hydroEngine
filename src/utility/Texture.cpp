#include "Texture.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include "../../includes/stb_image.h"
#include <string>
#include <vector>

glm::ivec2 Texture::lastCreatedImageSize = glm::ivec2(0);

Texture::Texture(const uint textureID, const glm::ivec2 size, const GLenum type)
    :  ID(textureID), size (size), type(type){}

uint Texture::textureFromFile(const char * filenameChar, const std::string &directory, GLint wrap, GLint filter) {
    std::string filename = std::string(filenameChar);
    filename = directory + '/' + filename;

    uint textureID = 0;
    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data){
        GLenum format;
        if (nrComponents == 1)format = GL_RED;
        else if (nrComponents == 4)format = GL_RGBA;
        else format = GL_RGB;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        setParameters(GL_TEXTURE_2D, wrap, filter);

        lastCreatedImageSize = glm::ivec2(width, height);

        stbi_image_free(data);
    }

    else std::cout << "Texture failed to load at path: " << filename << std::endl;

    return textureID;
}

uint Texture::hdrTextureFromFile(const char * filenameChar, const std::string &directory, GLint wrap, GLint filter) {
    std::string filename = std::string(filenameChar);
    filename = directory + '/' + filename;

    uint textureID = 0;
    int width, height, nrComponents;
    float * data = stbi_loadf(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
        setParameters(GL_TEXTURE_2D, wrap, filter);

        lastCreatedImageSize = glm::ivec2(width, height);

        stbi_image_free(data);
    }

    else std::cout << "Texture failed to load at path: " << filename << std::endl;

    return textureID;
}

void Texture::free(){
    glDeleteTextures(1, &ID);
}

void Texture::bind(const GLenum textureChannel) const
{
    glActiveTexture(textureChannel);
    glBindTexture(type, this->ID);
}

uint Texture::createTexture(glm::ivec2 size, GLint internalFormat, GLenum format,
                            GLenum type, const void * data, GLint wrap, GLint filter) {
    uint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, data);
    setParameters(GL_TEXTURE_2D, wrap, filter);
    glBindTexture(GL_TEXTURE_2D, 0);
    lastCreatedImageSize = size;

    return textureID;
}

uint Texture::createColorTexture(glm::vec3 color) {
    lastCreatedImageSize = glm::ivec2(1);
    return createTexture(glm::ivec2(1),  GL_RGB, GL_RGB, GL_FLOAT, &color[0]);
}

uint Texture::createCubemapTexture(glm::ivec2 size, GLint wrap, GLint filter) {
    uint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for (uint i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size.x, size.y, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    setParameters(GL_TEXTURE_CUBE_MAP, wrap, filter);
    lastCreatedImageSize = size;
    return textureID;
}

uint Texture::cubemapFromDirectory(const std::string &directory, GLint wrap, GLint filter){
    uint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    std::vector<std::string> faces;
    for (const auto & entry : std::filesystem::directory_iterator(directory))
        faces.push_back(entry.path());
    std::sort(faces.begin(), faces.end());

    int width, height, nrChannels;
    for (uint i = 0; i < faces.size(); i++){

        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if(data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                    width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            lastCreatedImageSize = glm::ivec2(width, height);
        }
        else std::cout << "Cubemap failed to load at path: " << faces[i] << "\n";
    }

    setParameters(GL_TEXTURE_CUBE_MAP, wrap, filter);

    return textureID;
}

void Texture::setParameters(GLenum type, GLint wrap, GLint filter) {
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, wrap);
    if (type == GL_TEXTURE_CUBE_MAP) glTexParameteri(type, GL_TEXTURE_WRAP_R, wrap);
}