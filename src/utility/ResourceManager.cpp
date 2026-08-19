#include "ResourceManager.hpp"
#include <iostream>

std::map<std::string, Shader*>    ResourceManager::shaders;
std::map<std::string, Texture*> ResourceManager::textures;
std::map<std::string, Utility::Font*> ResourceManager::fonts;
FT_Library ResourceManager::ft;

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

void ResourceManager::addFont(const std::string & name, const char * fontPath) {
    static bool initialized = false;
    if (!initialized)
        if (FT_Init_FreeType(&ft))
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
    auto * font = new Utility::Font();
    Utility::loadFont(ft, *font, fontPath);
    fonts.insert({name, font});
}
Utility::Font * ResourceManager::getFont(const std::string & name) {
    if (fonts.find(name) == fonts.end())
        throw std::invalid_argument("Font " + name + " does not exists");
    return fonts.at(name);
}

void ResourceManager::clear() {
    for (auto &[name, shader]: shaders) {
        delete shader;
    }
    for (auto &[name, texture]: textures) {
        delete texture;
    }
    for (auto &[name, font]: fonts) {
        delete font; //todo characters texture aren't getting deleted with glDeleteTextures()
    }
    shaders.clear();
    textures.clear();
    fonts.clear();
    FT_Done_FreeType(ft);
}