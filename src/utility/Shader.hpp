#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <vector>
#include <glm/glm.hpp>

class Shader{
    public :
        uint ID;
        static constexpr const char * directory = "../resources/shaders/";

        Shader(const char* vertexName, const char* fragmentName, const char* geometryName = nullptr,
            const char* tesselControlName = nullptr, const char* tesselEvalName = nullptr);
        Shader(const char* computeName);
        Shader(Shader & shader) = delete;
        ~Shader();

        void use() const;
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec4(const std::string &name, glm::vec4 values) const;
        void setVec3(const std::string &name, glm::vec3 values) const;
        void setVec2(const std::string &name, glm::vec2 values) const;
        void setMat4(const std::string &name, glm::mat4 matrix) const;
        void setUniformBlock(const std::string & name, int index) const;

    private:
        uint prepareShader(const char* fileName, uint shaderType);
        void readShaderFile(const char* path, std::ifstream & shaderFile, std::string &code);
        void printCompileStatus(uint shader, const std::string & name);
        void printLinkStatus(uint program);
        uint compileShader(uint shaderType, const char *shaderSource, const std::string & name);
        uint prepareProgramm(uint vertexShader, uint fragmentShader, uint geometryShader, uint tesselControlShader, uint tesselEvalShaderShader);

        //from https://stackoverflow.com/questions/78885511/how-can-i-use-include-in-a-glsl-file-using-c
        std::string findIncludeFile(const std::string& fileName, const std::vector<std::string>& includeDirs);
        std::string processIncludes(const std::string& input, const std::vector<std::string>& includeDirs);
};

#endif