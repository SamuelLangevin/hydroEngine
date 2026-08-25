#include "Sphere.hpp"

#include "Mesh.hpp"

#define PI 3.14159265359

uint Sphere::VAO, Sphere::VBO, Sphere::EBO;
int Sphere::latResolution = 20, Sphere::lonResolution = 20;

Sphere::Sphere()= default;

void Sphere::load(){
    if (VAO == 0) {
        std::vector<Mesh::Vertex> vertices;

        for (int i = 0; i <= latResolution; ++i) {
            float vertical = i / float(latResolution);
            float theta = glm::mix(-PI/2, PI/2, vertical);
            float coordY = sin(theta);
            float rad = cos(theta);

            for (int j = 0; j <= lonResolution; ++j) {
                float horizontal = j / float(lonResolution);
                float phi = glm::mix(-PI, PI, horizontal);
                float coordX = rad * cos(phi);
                float coordZ = rad * sin(phi);

                glm::vec3 normal = glm::normalize(glm::vec3(coordX, coordY, coordZ));
                Mesh::Vertex vertex;
                vertex.position = 0.5f * normal;
                vertex.normal = normal;
                vertex.texCoords = glm::vec2(horizontal, 1.0f - vertical);
                vertices.push_back(vertex);
            }
        }

        std::vector<uint> indices;
        int stride = lonResolution + 1;
        for (int i = 0; i < latResolution; ++i) {
            for (int j = 0; j < lonResolution; ++j) {
                uint a = i * stride + j;
                uint b = a + 1;
                uint c = a + stride;
                uint d = c + 1;
                indices.push_back(a);
                indices.push_back(c);
                indices.push_back(b);
                indices.push_back(b);
                indices.push_back(c);
                indices.push_back(d);
            }
        }
        std::vector<Mesh::Texture> textures;
        Mesh mesh(vertices, indices, textures);
        VAO = mesh.VAO;
        VBO = mesh.VBO;
        EBO = mesh.EBO;
    }
    glBindVertexArray(VAO);
}

void Sphere::free() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, & VAO);
}

void Sphere::draw(Shader * shader) const {
    load();
    setUniforms(shader, 0);
    glDrawElements(GL_TRIANGLES, lonResolution * latResolution * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

