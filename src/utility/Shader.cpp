#include "Shader.hpp"
#include <filesystem>
#include <iostream>
#include <regex>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include "../../includes/glad.h"

Shader::Shader(const uint programID) : ID(programID){}

uint Shader::createShader(const char* vertexName, const char* fragmentName, const char* geometryName,
    const char* tesselControlName, const char* tesselEvalName) {

    uint vertexShader = prepareShader(vertexName, GL_VERTEX_SHADER);
    uint fragmentShader = prepareShader(fragmentName, GL_FRAGMENT_SHADER);
    uint geometryShader = prepareShader(geometryName, GL_GEOMETRY_SHADER);
    uint tesselControlShader = prepareShader(tesselControlName, GL_TESS_CONTROL_SHADER);
    uint tesselEvalShader = prepareShader(tesselEvalName, GL_TESS_EVALUATION_SHADER);
    return prepareProgram(vertexShader, fragmentShader, geometryShader, tesselControlShader, tesselEvalShader);
}

uint Shader::createComputeShader(const char* computeName) {
    const uint computeShader = prepareShader(computeName, GL_COMPUTE_SHADER);
    const uint ID = glCreateProgram();
    glAttachShader(ID, computeShader);
    glLinkProgram(ID);
    printLinkStatus(ID);
    glDeleteShader(computeShader);
    return ID;
}

void Shader::free() {
    glDeleteProgram(ID);
}

uint Shader::prepareShader(const char* fileName, const uint shaderType){
    uint shader = 0;
    if (fileName != nullptr) {
        char filePath[100];
        strcpy(filePath,directory);
        strcat(filePath,fileName);

        std::string code;

        try {
            readFile(filePath, code);
            code = processIncludes(code,{directory});
        } catch (std::ifstream::failure&){std::cout << "ERROR::SHADER::FAILURE_TO_READ_FILE\n";}

        shader = compileShader(shaderType, code.c_str(), filePath);
    }

    return shader;
}

void Shader::printCompileStatus(const uint shader, const std::string & name){
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << name << " SHADER COMPILE ERROR\n" << infoLog << "\n";
    }
}

void Shader::printLinkStatus(const uint program){
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "PROGRAM" << " LINK ERROR\n" << infoLog << "\n";
    }
}

uint Shader::compileShader(const uint shaderType, const char *code, const std::string & name){
    uint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);
    printCompileStatus(shader, name);
    return shader;
}

uint Shader::prepareProgram(uint vertexShaderID, uint fragmentShaderID, uint geometryShaderID, uint tesselControlShaderID, uint tesselEvalShader){
    uint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderID);
    glAttachShader(shaderProgram, fragmentShaderID);
    if (geometryShaderID != 0) glAttachShader(shaderProgram, geometryShaderID);
    if (tesselControlShaderID != 0) glAttachShader(shaderProgram, tesselControlShaderID);
    if (tesselEvalShader != 0) glAttachShader(shaderProgram, tesselEvalShader);
    glLinkProgram(shaderProgram);
    printLinkStatus(shaderProgram);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    glDeleteShader(geometryShaderID);
    glDeleteShader(tesselControlShaderID);
    glDeleteShader(tesselEvalShader);
    return shaderProgram;
}

void Shader::readFile(const char* path, std::string &code){
    std::ifstream file;
    std::stringstream shaderStream;
    file.open(path);
    shaderStream << file.rdbuf();
    file.close();
    code = shaderStream.str();
}

std::string Shader::findIncludeFile(const std::string& fileName, const std::vector<std::string>& includeDirs) {
    if (std::filesystem::exists(fileName)) {
        return fileName;
    }

    for (const auto& dir : includeDirs) {
        std::filesystem::path filePath = std::filesystem::path(dir) / fileName;
        if (std::filesystem::exists(filePath)) {
            return filePath.string();
        }
    }

    throw std::runtime_error("File not found: " + fileName);
}

std::string Shader::processIncludes(const std::string& input, const std::vector<std::string>& includeDirs) {
    const std::regex includeRegex(R"(#include\s*["<](.*?)[">])");
    std::smatch match;
    std::string output = input;
    std::string::const_iterator searchStart(output.cbegin());

    while (std::regex_search(searchStart, output.cend(), match, includeRegex)) {
        std::string includeFile = match[1].str();
        std::string fileContent;
        try {
            std::string filePath = findIncludeFile(includeFile, includeDirs);
            readFile(filePath.c_str(), fileContent);
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            fileContent = "";
        }

        const long matchPos = match.position(0) + (searchStart - output.cbegin());
        output.replace(matchPos, match.length(0), fileContent);
        searchStart = output.cbegin() + matchPos;
    }

    return output;
}

void Shader::use() const{ glUseProgram(ID);}

void Shader::setBool(const std::string &name, const bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec4(const std::string &name, glm::vec4 values) const{
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform4f(location, values.x, values.y , values.z, values.w);
}

void Shader::setVec3(const std::string &name, glm::vec3 values) const{
	int location = glGetUniformLocation(ID, name.c_str());
    glUniform3f(location, values.x, values.y , values.z);
}

void Shader::setVec2(const std::string &name, glm::vec2 values) const{
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform2f(location, values.x, values.y);
}

void Shader::setMat4(const std::string &name, glm::mat4 matrix) const{
     int location = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformBlock(const std::string & name, int index) const {
    uint matricesBlock = glGetUniformBlockIndex(ID, name.c_str());
    glUniformBlockBinding(ID, matricesBlock, index);
}
