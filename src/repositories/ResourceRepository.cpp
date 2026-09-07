#include "ResourceRepository.hpp"
#include <iostream>

std::map<std::string, Shader> ResourceRepository::shaders;
std::map<std::string, Texture> ResourceRepository::textures;


void ResourceRepository::addShader(const std::string &name, const Shader shader) {
    shaders.insert({name, shader});
}

Shader ResourceRepository::getShader(const std::string & name) {
    if (shaders.find(name) == shaders.end())
        throw std::invalid_argument("Shader " + name + " does not exists");
    return shaders.at(name);
}

void ResourceRepository::addTexture(const std::string & name, Texture texture) {
    textures.insert({name, texture});
}

Texture ResourceRepository::getTexture(const std::string & name) {
    if (textures.find(name) == textures.end())
        throw std::invalid_argument("Texture " + name + " does not exists");
    return textures.at(name);
}


void ResourceRepository::clear() {
    for (auto &[name, shader]: shaders) {
        glDeleteProgram(shader.getID());
    }
    for (auto &[name, texture]: textures) {
        uint id = texture.getID();
        glDeleteTextures(1, &id);
    }
    shaders.clear();
    textures.clear();
}