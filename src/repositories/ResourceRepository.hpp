#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <string>

#include "../utility/Texture.hpp"
#include "../../includes/glad.h"
#include "../utility/Shader.hpp"

//todo allow to store framebuffers
//todo allow to store meshes{VAO, VBO, EBO} ?
/** \class ResourceRepository
 * Singleton managing the shaders and textures creation, usage and destruction.
 */
class ResourceRepository{
    public:

        ResourceRepository() = delete;

        /**
         * Creates and stores a shader object by name from an existing OpenGL program.
         * @param name given to the shader
         * @param programID
         */
        static void addShader(const std::string & name, uint programID);

        /**
         * @param name of the shader
         * @returns a copy the named shader
         * @throws std::invalid_argument if there is no shader linked to the given name
         */
        [[nodiscard]] static Shader getShader(const std::string & name);

        /**
         * Creates and stores a texture object by name from an existing OpenGL texture.
         * @param name given to the texture
         * @param textureID
         * @param size the image's height and width.
         * @param type 2D ? Cubemap ?
         */
        static void addTexture(const std::string & name, uint textureID, glm::ivec2 size, GLenum type);

        /**
        * @param name of the texture
        * @returns a copy of the named texture
        * @throws std::invalid_argument if there is no texture linked to the given name
        */
        [[nodiscard]] static Texture getTexture(const std::string & name);

        /**
         * clear stored shaders and textures and deletes their OpenGL resources.
         */
        static void clear();

    private:

        static std::map<std::string, Shader> shaders; /**< Shaders stored by name*/
        static std::map<std::string, Texture> textures; /**< Textures stored by name*/

};

#endif
