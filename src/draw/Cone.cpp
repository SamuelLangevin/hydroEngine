#include "Cone.hpp"

#include "Mesh.hpp"
#include <glm/gtc/type_ptr.hpp>

#define PI 3.14159265359

uint Cone::VAO;
int Cone::lonResolution = 40;

Cone::Cone()= default;

void Cone::load(){
    if (VAO == 0) {
        std::vector<Mesh::Vertex> vertices;
        Mesh::Vertex topVertex; // Normals will be approximated
        topVertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        topVertex.position = 0.5f * topVertex.normal;
        topVertex.texCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(topVertex);

        Mesh::Vertex bottomVertex;
        bottomVertex.normal = glm::vec3(0.0f, -1.0f, 0.0f);
        bottomVertex.position = 0.5f * bottomVertex.normal;
        bottomVertex.texCoords = glm::vec2(0.0f, 0.0f);
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
            sideVertex.texCoords = glm::vec2(0.0f, 0.0f);
            vertices.push_back(sideVertex);

            Mesh::Vertex baseVertex;
            baseVertex.position = pos;
            baseVertex.normal = glm::vec3(0.0f, -1.0f, 0.0f);
            baseVertex.texCoords = glm::vec2(0.0f, 0.0f);
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
    }
    glBindVertexArray(VAO);
}

void Cone::draw(Shader * shader) const {
    load();
    setUniforms(shader);
    glDrawElements(GL_TRIANGLES, lonResolution * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
