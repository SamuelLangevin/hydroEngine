#ifndef MESH_HPP
#define MESH_HPP

#include <glm/glm.hpp>
#include <vector>

/** \class Mesh
 * Allows to generate and draw a custom mesh from vertices, indices and textures.
 */
class Mesh {
private:
    uint VAO{}; /**< ID of the vertex array object storing the layout of the mesh. */
    uint VBO{}; /**< ID of the vertex buffer object storing the vertices of the mesh. */
    uint EBO{}; /**< ID of the element buffer object storing the vertices' draw order. */
public:

    /** Vertex structure to generate the VBO */
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    uint vertexDrawCount{}; /**< Vertices drawn on a draw() call. */

    /** Deletes the VAO, VBO and EBO */
    void free();

    [[nodiscard]] uint getVaoID() const { return VAO; }

    /**
     * Generates the VAO, VBO and EBO
     * @param vertices
     * @param indices
     */
    Mesh(const std::vector<Vertex> &vertices, const std::vector<uint> &indices);

    Mesh() = default;

    void draw() const;
};

#endif