#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <string>

#include "Texture.hpp"
#include "../../includes/glad.h"
#include "Shader.hpp"
#include "Utility.hpp"


class ResourceManager{
public:

    ResourceManager() = delete;

    static void loadShader(const std::string & name, const char * vsPath, const char * fsPath,
        const char * gsPath = nullptr, const char * tcsPath = nullptr, const char * tesPath = nullptr);
    static void loadComputeShader(const std::string & name, const char * csPath);
    [[nodiscard]] static Shader * getShader(const std::string & name);

    static void loadTexture(const std::string & name, const char * filenameChar, const std::string &directory, GLint wrap = GL_REPEAT, GLint filters = GL_LINEAR);
    static void loadHDRTexture(const std::string & name, const char * filenameChar, const std::string &directory);
    static void loadCubemap(const std::string & name, const std::string &directory);
    static void addTexture(const std::string & name, uint textureID, GLenum type);
    [[nodiscard]] static Texture * getTexture(const std::string & name);

    static void addFont(const std::string & name, const char * fontPath);
    [[nodiscard]] static Utility::Font * getFont(const std::string & name);

    static void clear();
private:
    static std::map<std::string, Shader*>    shaders;
    static std::map<std::string, Texture*> textures;
    static std::map<std::string, Utility::Font*> fonts;
    static FT_Library ft;

};

#endif
