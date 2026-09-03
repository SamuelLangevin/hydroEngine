#include "ResourceRepository.hpp"
#include <iostream>

std::map<std::string, Shader> ResourceRepository::shaders;
std::map<std::string, Texture> ResourceRepository::textures;


void ResourceRepository::addShader(const std::string &name, const uint programID) {
    shaders.insert({name, Shader(programID)});
}

Shader ResourceRepository::getShader(const std::string & name) {
    if (shaders.find(name) == shaders.end())
        throw std::invalid_argument("Shader " + name + " does not exists");
    return shaders.at(name);
}

void ResourceRepository::addTexture(const std::string & name, const uint textureID, const glm::ivec2 size, const GLenum type) {
    textures.insert({name, Texture(textureID, size, type)});
}

Texture ResourceRepository::getTexture(const std::string & name) {
    if (textures.find(name) == textures.end())
        throw std::invalid_argument("Texture " + name + " does not exists");
    return textures.at(name);
}


void ResourceRepository::clear() {
    for (auto &[name, shader]: shaders) {
        shader.free();
    }
    for (auto &[name, texture]: textures) {
        texture.free();
    }
    shaders.clear();
    textures.clear();
}