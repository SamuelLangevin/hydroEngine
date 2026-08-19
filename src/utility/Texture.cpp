#include "Texture.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include "../../includes/stb_image.h"
#include <string>
#include <vector>

Texture::Texture(uint textureID, GLenum type)
    :  ID(textureID), type(type){}

uint Texture::textureFromFile(const char * filenameChar, const std::string &directory, GLint wrap, GLint filters) {
    std::string filename = std::string(filenameChar);
    filename = directory + '/' + filename;

    uint textureID = 0;
    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)format = GL_RED;
        else if (nrComponents == 4)format = GL_RGBA;
        else format = GL_RGB;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filters);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filters);

        stbi_image_free(data);
    }

    else std::cout << "Texture failed to load at path: " << filename << std::endl;

    return textureID;
}

uint Texture::hdrTextureFromFile(const char * filenameChar, const std::string &directory) {
    std::string filename = std::string(filenameChar);
    filename = directory + '/' + filename;

    uint textureID = 0;
    int width, height, nrComponents;
    float *data = stbi_loadf(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }

    else std::cout << "Texture failed to load at path: " << filename << std::endl;

    return textureID;
}

Texture::~Texture() {
    glDeleteTextures(1, &ID);
}

void Texture::bind(GLenum textureChannel) const
{
    glActiveTexture(textureChannel);
    glBindTexture(type, this->ID);
}

uint Texture::createTexture(glm::ivec2 resolution, GLint internalFormat, GLenum format, GLenum type, GLint wrap, GLint filter, const void * data) {
    uint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, resolution.x, resolution.y, 0, format, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

uint Texture::createColorTexture(glm::vec3 color) {
    return createTexture(glm::ivec2(1),  GL_RGB16F, GL_RGB, GL_FLOAT, GL_REPEAT,GL_LINEAR, &color[0]);
}

uint Texture::createCubemapTexture(glm::ivec2 resolution) {
    uint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for (uint i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, resolution.x, resolution.y, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return textureID;
}

uint Texture::cubemapFromDirectory(const std::string &directory){
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
        }
        else std::cout << "Cubemap failed to load at path: " << faces[i] << "\n";
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}