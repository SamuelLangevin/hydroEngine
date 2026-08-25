#ifndef MESH_HPP
#define MESH_HPP

#include <glm/glm.hpp>
#include "../utility/Shader.hpp"
#include <vector>

class Mesh {
    public:

        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoords;
        };

        struct Texture {
            uint id;
            std::string type;
            std::string path;
        };

        std::vector<Vertex> vertices;
        std::vector<uint> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<uint> indices,
            std::vector<Texture> textures);

        void free();
        void draw(const Shader & shader) const;

        uint VAO, VBO, EBO;

    private:
        void setupMesh();

};

#endif