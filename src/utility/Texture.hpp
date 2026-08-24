#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP
#include <string>
#include <vec2.hpp>
#include <vec3.hpp>

#include "../../includes/glad.h"

/** \class Texture
 * Represents an OpenGL texture.
 */
class Texture{
public:

    const uint ID; /**< The OpenGL texture ID. */
    const glm::ivec2 size; /**< The width and height of the image. */
    const GLenum type; /**< The OpenGL image type; GL_TEXTURE_2D ? GL_TEXTURE_CUBE_MAP ?*/

    static glm::ivec2 lastCreatedImageSize; /**< The size of the last created image by this class functions.*/

    /**
     * Creates a texture object from an OpenGL texture and its type.
     * @param textureID The OpenGL texture ID to associate.
     * @param size The width and height of the image.
     * @param type Defaults to GL_TEXTURE_2D
     */
    explicit Texture(uint textureID, glm::ivec2 size, GLenum type);

    Texture() = delete;
    ~Texture() = default;

    /** Deletes the associated OpenGL texture. */
    void free();

    /**
     * Binds the texture to the specified texture channel.
     * @param textureChannel
     */
    void bind(GLenum textureChannel) const;

    /**
     * Loads and creates a basic texture from a filename and applies the
     * passed wrap and filter parameters.
     * @param filenameChar
     * @param directory path where the image is located
     * @param wrap method to apply. Defaults to GL_CLAMP_TO_EDGE
     * @param filter method to apply. Defaults to GL_LINEAR
     * @returns the generated OpenGL texture ID
     */
    static uint textureFromFile(const char * filenameChar, const std::string &directory,
                                GLint wrap = GL_CLAMP_TO_EDGE, GLint filter = GL_LINEAR);

    /**
     * Loads and creates an hdr texture from a filename.
     * @param filenameChar
     * @param directory path where the image is located
     * @param wrap method to apply. Defaults to GL_CLAMP_TO_EDGE
     * @param filter method to apply. Defaults to GL_LINEAR
     * @returns the generated OpenGL texture ID
     */
    static uint hdrTextureFromFile(const char * filenameChar, const std::string &directory,
                                    GLint wrap = GL_CLAMP_TO_EDGE, GLint filter = GL_LINEAR);

    /**
     * Creates a 2D texture from preloaded or no data.
     * @param size of the image
     * @param internalFormat see the openGL documentation
     * @param format see the openGL documentation
     * @param type 2D ? Cubemap ?
     * @param data the preloaded data. Defaults to nullptr
     * @param wrap method to apply. Defaults to GL_CLAMP_TO_EDGE
     * @param filter method to apply. Defaults to GL_LINEAR
     * @returns the generated OpenGL texture ID
     */
    static uint createTexture( glm::ivec2 size, GLint internalFormat, GLenum format, GLenum type,
                const void * data = nullptr, GLint wrap = GL_CLAMP_TO_EDGE, GLint filter = GL_LINEAR);

    /**
     * Creates an empty cubemap with the given resolution.
     * @param size of each of its sides
     * @param wrap method to apply. Defaults to GL_CLAMP_TO_EDGE
     * @param filter method to apply. Defaults to GL_LINEAR
     * @returns the generated OpenGL texture ID
     */
    static uint createCubemapTexture(glm::ivec2 size, GLint wrap = GL_CLAMP_TO_EDGE, GLint filter = GL_LINEAR);

    /**
     * Loads and creates a cubemap from given directory's images.
     * @param directory to use side images from.
     * It must only contain the image from the cubemap.
     * For simplicity the images will be sorted alphabetically and are expected to be
     * found in this order : right, left, top, bottom, back, front.
     * @param wrap method to apply. Defaults to GL_CLAMP_TO_EDGE
     * @param filter method to apply. Defaults to GL_LINEAR
     * @returns the generated OpenGL texture ID
     */
    static uint cubemapFromDirectory(const std::string &directory, GLint wrap = GL_CLAMP_TO_EDGE, GLint filter = GL_LINEAR);

    /**
     * Creates a 1 pixel color texture.
     * @param color
     * @return the generated OpenGL texture ID.
     */
    static uint createColorTexture(glm::vec3 color);

    /**
     * Applies the given wrap and filter methods to the bound texture.
     * @param type
     * @param wrap
     * @param filter
     */
    static void setParameters(GLenum type, GLint wrap, GLint filter);

    /**
     * https://stackoverflow.com/questions/5844858/how-to-take-screenshot-in-opengl
     * takes the color attachment of the current framebuffer and writes to an image to the corresponding name.
     *
     * Bash script to convert .tga images to .png :
    * Install the image converter

    sudo apt install graphicsmagick-imagemagick-compat  # version 1.4+really1.3.42-1

    for oldFileName in *.tga; do
        [ -f "$oldFileName" ] || break
        newFileName=${oldFileName//.tga/.png}
        convert $oldFileName $newFileName
        rm $oldFileName
        echo "Converted $oldFileName to $newFileName"
    done

    * @param filename name of the image to generate
    * @param size the size of the image to generate
    */
    static void saveTextureToFile(const std::string& filename, glm::ivec2 size);

private:;

};

#endif