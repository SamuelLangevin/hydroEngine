#include "Texture.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include "../../includes/stb_image.h"
#include <string>
#include <vector>
#include <GL/glext.h>

Texture::Texture(const uint textureID, const glm::ivec2 size, const GLenum type)
    :  ID(textureID), size (size), type(type){}

uint Texture::getID() const { return ID;}

glm::ivec2 Texture::getSize() const {  return size;}

GLenum Texture::getType() const { return type;}

Texture Texture::textureFromFile(const char * filenameChar, const std::string &directory, GLint wrap, GLint filter) {
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

        stbi_image_free(data);
    }

    else std::cout << "Texture failed to load at path: " << filename << std::endl;

    return Texture(textureID, glm::ivec2(width, height), GL_TEXTURE_2D);
}

Texture Texture::hdrTextureFromFile(const char * filenameChar, const std::string &directory, GLint wrap, GLint filter) {
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

        stbi_image_free(data);
    }

    else std::cout << "Texture failed to load at path: " << filename << std::endl;

    return Texture(textureID, glm::ivec2(width, height), GL_TEXTURE_2D);
}

void Texture::bind(const Shader & shader, const std::string &name, int channel) const
{
    shader.setInt(name, channel);
    glActiveTexture(GL_TEXTURE0 + channel);
    glBindTexture(type, this->ID);
}

Texture Texture::createTexture(glm::ivec2 size, GLint internalFormat, GLenum format,
                            GLenum type, const void * data, GLint wrap, GLint filter) {
    uint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, data);
    setParameters(GL_TEXTURE_2D, wrap, filter);
    glBindTexture(GL_TEXTURE_2D, 0);

    return Texture(textureID, size, GL_TEXTURE_2D);
}

Texture Texture::createColorTexture(glm::vec3 color) {
    return createTexture(glm::ivec2(1),  GL_RGB, GL_RGB, GL_FLOAT, &color[0]);
}

Texture Texture::createCubemapTexture(glm::ivec2 size, GLint wrap, GLint filter) {
    uint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for (uint i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size.x, size.y, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    setParameters(GL_TEXTURE_CUBE_MAP, wrap, filter);

    return Texture(textureID, size, GL_TEXTURE_CUBE_MAP);
}

Texture Texture::cubemapFromDirectory(const std::string &directory, GLint wrap, GLint filter){
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
        }
        else std::cout << "Cubemap failed to load at path: " << faces[i] << "\n";
    }

    setParameters(GL_TEXTURE_CUBE_MAP, wrap, filter);

    return Texture(textureID, glm::ivec2(width, height), GL_TEXTURE_CUBE_MAP);
}

void Texture::setParameters(GLenum type, GLint wrap, GLint filter) {
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, wrap);
    if (type == GL_TEXTURE_CUBE_MAP) glTexParameteri(type, GL_TEXTURE_WRAP_R, wrap);
}

void Texture::saveTextureToFile(const std::string& filename, glm::ivec2 size) {
    const int numberOfPixels = size.x * size.y * 3;
    unsigned char pixels[numberOfPixels];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, size.x, size.y, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

    FILE *outputFile = fopen(filename.c_str(), "w");
    short header[] = {0, 2, 0, 0, 0, 0, (short) size.x, (short) size.y, 24};

    fwrite(&header, sizeof(header), 1, outputFile);
    fwrite(pixels, numberOfPixels, 1, outputFile);
    fclose(outputFile);

    std::cout << "Finish writing to " + filename + "\n";
}