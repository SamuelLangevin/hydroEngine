#include "ResourceManager.hpp"
#include <iostream>

std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture> ResourceManager::textures;


void ResourceManager::addShader(const std::string &name, const uint programID) {
    shaders.insert({name, Shader(programID)});
}

Shader ResourceManager::getShader(const std::string & name) {
    if (shaders.find(name) == shaders.end())
        throw std::invalid_argument("Shader " + name + " does not exists");
    return shaders.at(name);
}

void ResourceManager::addTexture(const std::string & name, const uint textureID, const glm::ivec2 size, const GLenum type) {
    textures.insert({name, Texture(textureID, size, type)});
}

Texture ResourceManager::getTexture(const std::string & name) {
    if (textures.find(name) == textures.end())
        throw std::invalid_argument("Texture " + name + " does not exists");
    return textures.at(name);
}


void ResourceManager::clear() {
    for (auto &[name, shader]: shaders) {
        shader.free();
    }
    for (auto &[name, texture]: textures) {
        texture.free();
    }
    shaders.clear();
    textures.clear();
}