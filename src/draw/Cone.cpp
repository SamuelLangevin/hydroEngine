#include "Cone.hpp"

#include "Mesh.hpp"
#include <glm/gtc/type_ptr.hpp>

#define PI 3.14159265359

uint Cone::VAO, Cone::VBO, Cone::EBO;
const int Cone::lonResolution = 40;

void Cone::free() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, & VAO);
}

void Cone::load(){
    if (VAO == 0) {
        //todo texture coordinates are not calculated
        //fixme Normals are approximated

        std::vector<Mesh::Vertex> vertices;
        Mesh::Vertex topVertex;
        topVertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        topVertex.position = 0.5f * topVertex.normal;
        vertices.push_back(topVertex);

        Mesh::Vertex bottomVertex;
        bottomVertex.normal = glm::vec3(0.0f, -1.0f, 0.0f);
        bottomVertex.position = 0.5f * bottomVertex.normal;
        vertices.push_back(bottomVertex);

        for (int i = 0; i <= lonResolution; ++i) {
            float vertical = i / float(lonResolution);
            float theta = glm::mix(-PI, PI, vertical);
            float coordX = cos(theta);
            float coordZ = sin(theta);

            glm::vec3 pos = 0.5f * glm::vec3(coordX, -1.0f, coordZ);

            Mesh::Vertex sideVertex;
            sideVertex.position = pos;
            sideVertex.normal = normalize(glm::vec3(coordX, 1.5f, coordZ));
            vertices.push_back(sideVertex);

            Mesh::Vertex baseVertex;
            baseVertex.position = pos;
            baseVertex.normal = glm::vec3(0.0f, -1.0f, 0.0f);
            vertices.push_back(baseVertex);
        }


        std::vector<uint> indices;
            for (int i = 1; i <= lonResolution; ++i) {
                uint a = 0;
                uint b = i * 2;
                uint c = (b%(lonResolution*2))+2;
                indices.push_back(a);
                indices.push_back(c);
                indices.push_back(b);

                uint aa = 1;
                uint bb = i * 2 +1;
                uint cc = (bb%(lonResolution*2))+2;
                indices.push_back(cc);
                indices.push_back(aa);
                indices.push_back(bb);
        }
        std::vector<Mesh::Texture> textures;
        Mesh mesh(vertices, indices, textures);
        VAO = mesh.VAO;
        VBO = mesh.VBO;
        EBO = mesh.EBO;
    }
    glBindVertexArray(VAO);
}

void Cone::draw(const Shader & shader) const {
    load();
    setUniforms(shader, 0);
    glDrawElements(GL_TRIANGLES, lonResolution * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
