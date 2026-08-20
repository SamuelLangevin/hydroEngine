#include "ResourceManager.hpp"
#include <iostream>

std::map<std::string, Shader*>    ResourceManager::shaders;
std::map<std::string, Texture*> ResourceManager::textures;

void ResourceManager::loadShader(const std::string & name, const char * vsPath, const char * fsPath,
    const char * gsPath, const char * tcsPath, const char * tesPath) {
    shaders.insert({name, new Shader(vsPath, fsPath, gsPath, tcsPath, tesPath)});
}

void ResourceManager::loadComputeShader(const std::string & name, const char * csPath) {
    shaders.insert({name, new Shader(csPath)});
}

Shader * ResourceManager::getShader(const std::string & name) {
    if (shaders.find(name) == shaders.end())
        throw std::invalid_argument("Shader " + name + " does not exists");
    return shaders.at(name);
}

void ResourceManager::loadTexture(const std::string & name, const char * filenameChar, const std::string &directory, GLint wrap, GLint filters) {
    uint textureID = Texture::textureFromFile(filenameChar, directory, wrap, filters);
    addTexture(name, textureID, GL_TEXTURE_2D);
}

void ResourceManager::loadHDRTexture(const std::string & name, const char * filenameChar, const std::string &directory) {
    uint textureID = Texture::hdrTextureFromFile(filenameChar, directory);
    addTexture(name, textureID, GL_TEXTURE_2D);
}

void ResourceManager::loadCubemap(const std::string & name, const std::string &directory) {
    uint textureID = Texture::cubemapFromDirectory(directory);
    addTexture(name, textureID, GL_TEXTURE_CUBE_MAP);
}

void ResourceManager::addTexture(const std::string & name, uint textureID, GLenum type) {
    textures.insert({name, new Texture(textureID, type)});
}

Texture * ResourceManager::getTexture(const std::string & name) {
    if (textures.find(name) == textures.end())
        throw std::invalid_argument("Texture " + name + " does not exists");
    return textures.at(name);
}


void ResourceManager::clear() {
    for (auto &[name, shader]: shaders) {
        delete shader;
    }
    for (auto &[name, texture]: textures) {
        delete texture;
    }
    shaders.clear();
    textures.clear();
}