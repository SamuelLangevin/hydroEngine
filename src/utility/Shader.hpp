#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <vector>
#include <glm/glm.hpp>


/** \class Shader
 * Encapsulates OpenGL's programs.
 */
class Shader{
    public :
        /** Shaders directory path. */
        static constexpr const char * directory = "../resources/shaders/";

        /**
         * Creates a render pipeline shader.
         * @param vertexName vertex shader file name
         * @param fragmentName fragment shader file name
         * @param geometryName (optional) geometry shader file name
         * @param tesselControlName (optional) Tessellation control shader file name
         * @param tesselEvalName (optional) Tessellation evaluation shader file name
         */
        Shader(const char* vertexName, const char* fragmentName, const char* geometryName = nullptr,
               const char* tesselControlName = nullptr, const char* tesselEvalName = nullptr);

        /**
         * Creates a compute shader.
         * @param computeName compute shader file name
         */
        explicit Shader(const char* computeName);
        Shader(Shader & shader) = delete;

        /** Deletes the associated OpenGL program. */
        ~Shader();

        /** Binds the associated OpenGL program. */
        void use() const;

        /**
         * Sets the boolean value of the named uniform from this shader.
         * @param name of the uniform
         * @param value
         */
        void setBool(const std::string &name, bool value) const;

        /**
        * Sets the integer value of the named uniform from this shader.
        * @param name of the uniform
        * @param value
        */
        void setInt(const std::string &name, int value) const;

        /**
        * Sets the float value of the named uniform from this shader.
        * @param name of the uniform
        * @param value
        */
        void setFloat(const std::string &name, float value) const;

        /**
        * Sets the glsl vec4 values of the named uniform from this shader.
        * @param name of the uniform
        * @param values
        */
        void setVec4(const std::string &name, glm::vec4 values) const;

        /**
        * Sets the glsl vec3 values of the named uniform from this shader.
        * @param name of the uniform
        * @param values
        */
        void setVec3(const std::string &name, glm::vec3 values) const;

        /**
        * Sets the glsl vec2 values of the named uniform from this shader.
        * @param name of the uniform
        * @param values
        */
        void setVec2(const std::string &name, glm::vec2 values) const;

        /**
        * Sets the glsl mat4 matrix of the named uniform from this shader.
        * @param name of the uniform
        * @param matrix
        */
        void setMat4(const std::string &name, glm::mat4 matrix) const;

        /**
        * Sets the index of the named uniform block from this shader.
        * @param name of the uniform
        * @param index
        */
        void setUniformBlock(const std::string & name, int index) const;

    private:
        uint ID = 0; /**< The OpenGL program ID. */

        /**
         * Generates an OpenGL shader from compiled code.
         * @param fileName file name of the partial shader.
         * @param shaderType shader type/stage
         * @returns the OpenGL shader ID
         */
        static uint prepareShader(const char* fileName, uint shaderType);

        /**
         * Reads the content of a specified file
         * @param path the full file path
         * @param code the file's content (return param)
         */
        static void readFile(const char* path, std::string &code);

        /**
         * Prints shader compiling errors
         * @param shader OpenGL shader ID
         * @param name of the shader file
         */
        static void printCompileStatus(uint shader, const std::string & name);

        /**
         * Prints program linking errors
         * @param program OpenGL program ID
         */
        static void printLinkStatus(uint program);

        /**
         * Compiles a shader
         * @param shaderType the shader type/stage
         * @param code
         * @param name the shader file name
         * @returns the OpenGL shader ID
         */
        static uint compileShader(uint shaderType, const char *code, const std::string & name);

        /**
         * Creates an OpenGL program and links its shaders
         * @param vertexShaderID
         * @param fragmentShaderID
         * @param geometryShaderID
         * @param tesselControlShaderID
         * @param tesselEvalShaderID
         * @returns the OpenGL program ID
         */
        static uint prepareProgram(uint vertexShaderID, uint fragmentShaderID, uint geometryShaderID, uint tesselControlShaderID, uint tesselEvalShaderID);

        //from https://stackoverflow.com/questions/78885511/how-can-i-use-include-in-a-glsl-file-using-c
        static std::string findIncludeFile(const std::string& fileName, const std::vector<std::string>& includeDirs);
        static std::string processIncludes(const std::string& input, const std::vector<std::string>& includeDirs);
};

#endif