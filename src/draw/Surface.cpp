#include "Surface.hpp"
#include "../utility/Utility.hpp"

uint Surface::resolution = 20;

Surface::Surface(const glm::ivec2 size) : size(size) {
    std::vector<float> vertices;

    for (int i = 0; i < resolution-1; ++i) {
        for (int j = 0; j < resolution-1; ++j) {
            vertices.push_back(-size.x/2.0f + size.x*i/(float)resolution);
            vertices.push_back(0.0f);
            vertices.push_back(-size.y/2.0f + size.y*j/(float)resolution);
            vertices.push_back(i / (float)resolution);
            vertices.push_back(j / (float)resolution);

            vertices.push_back(-size.x/2.0f + size.x*(i+1)/(float)resolution);
            vertices.push_back(0.0f);
            vertices.push_back(-size.y/2.0f + size.y*j/(float)resolution);
            vertices.push_back((i+1) / (float)resolution);
            vertices.push_back(j / (float)resolution);

            vertices.push_back(-size.x/2.0f + size.x*i/(float)resolution);
            vertices.push_back(0.0f); // v.y
            vertices.push_back(-size.y/2.0f + size.y*(j+1)/(float)resolution);
            vertices.push_back(i / (float)resolution);
            vertices.push_back((j+1) / (float)resolution);

            vertices.push_back(-size.x/2.0f + size.x*(i+1)/(float)resolution);
            vertices.push_back(0.0f);
            vertices.push_back(-size.y/2.0f + size.y*(j+1)/(float)resolution);
            vertices.push_back((i+1) / (float)resolution);
            vertices.push_back((j+1) / (float)resolution);
        }
    }

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

Surface::~Surface() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Surface::draw(const Shader & shader) const {
    glBindVertexArray(VAO);
    setMatricesUniforms(shader);
    glDrawArrays(GL_PATCHES, 0, 4*resolution*resolution);
}

