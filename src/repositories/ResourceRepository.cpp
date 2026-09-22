#include "ResourceRepository.hpp"
#include <iostream>

std::map<std::string, Shader> ResourceRepository::shaders;
std::map<std::string, Texture> ResourceRepository::textures;
std::map<std::string, Mesh> ResourceRepository::meshes;


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

void ResourceRepository::addMesh(const std::string &name, Mesh mesh) {
    meshes.insert({name, mesh});
}

MeshRef ResourceRepository::getMesh(const std::string &name) {
    if (meshes.find(name) == meshes.end())
        throw std::invalid_argument("Mesh " + name + " does not exists");
    Mesh mesh = meshes.at(name);
    return MeshRef{mesh.getVaoID(), mesh.vertexDrawCount};
}

void ResourceRepository::clear() {
    for (auto &[name, shader]: shaders) {
        glDeleteProgram(shader.getID());
    }
    for (auto &[name, texture]: textures) {
        uint id = texture.getID();
        glDeleteTextures(1, &id);
    }
    for (auto &[name, mesh]: meshes) {
        mesh.free();
    }

    shaders.clear();
    textures.clear();
    meshes.clear();
}
