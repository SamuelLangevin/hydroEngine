#ifndef MESH_HPP
#define MESH_HPP

#include <glm/glm.hpp>
#include "../../resources/Shader.hpp"
#include <vector>

/** \class Mesh
 * Allows to generate and draw a custom mesh from vertices, indices and textures.
 */
class Mesh {
public:

    /** Vertex structure to generate the VBO */
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    /** Texture structure*/
    struct Texture {
        uint id; /**< OpenGL texture ID */
        std::string type; /**< diffuse or specular */
        std::string path; /**< file path*/
    };

    uint vertexDrawCount; /**< Vertices drawn on a draw() call. */
    std::vector<Texture> textures; /**< Textures used*/

    uint VAO{}; /**< ID of the vertex array object storing the layout of the mesh. */
    uint VBO{}; /**< ID of the vertex buffer object storing the vertices of the mesh. */
    uint EBO{}; /**< ID of the element buffer object storing the vertices' draw order. */

    /** Deletes the VAO, VBO and EBO */
    void free();

    /**
     * Generates the VAO, VBO and EBO
     * @param vertices
     * @param indices
     * @param textures
     */
    Mesh(const std::vector<Vertex> &vertices, const std::vector<uint> &indices,
         std::vector<Texture> textures);

    /**
     * Draws the mesh
     * @param shader
     */
    void draw(const Shader &shader) const;
};

#endif