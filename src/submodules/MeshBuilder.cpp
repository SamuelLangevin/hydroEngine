#include "MeshBuilder.hpp"
#include <stdexcept>

#include "../repositories/ResourceRepository.hpp"

#define PI 3.14159

MeshRef MeshBuilder::getSphereMesh() {
    try {
        return ResourceRepository::getMesh("sphere");
    } catch (std::invalid_argument e) {
        return createSphereMesh();
    }
}

MeshRef MeshBuilder::getBoxMesh() {
    try {
        return ResourceRepository::getMesh("box");
    } catch (std::invalid_argument e) {
        return createBoxMesh();
    }
}

MeshRef MeshBuilder::getRectangleMesh() {
    try {
        return ResourceRepository::getMesh("rectangle");
    } catch (std::invalid_argument e) {
        return createRectangleMesh();
    }
}

MeshRef MeshBuilder::getSurfaceMesh() {
    try {
        return ResourceRepository::getMesh("surface");
    } catch (std::invalid_argument e) {
        return createSurfaceMesh();
    }
}

MeshRef MeshBuilder::createSphereMesh() {
    std::vector<Mesh::Vertex> vertices;
    constexpr int latResolution = 20;
    constexpr int lonResolution = 20;

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

    ResourceRepository::addMesh("sphere", Mesh(vertices, indices));

    return ResourceRepository::getMesh("sphere");
}

MeshRef MeshBuilder::createBoxMesh() {
    std::vector<Mesh::Vertex> vertices = {
        //front
        {{0.5f,  0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},//0
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},//1
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},//2
        {{-0.5f,  0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},//3

       //bottom
        {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},//4
        {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},//5
        {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},//6
       {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},//7

       //back
        {{0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f},{ 0.0f, 1.0f}},//8
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},//9
       {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},//10
       {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},//11

       //top
        {{0.5f,  0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},//12
       {{-0.5f,  0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},//13
       {{0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},//14
       {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},//15

       //right
        {{0.5f,  0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},//16
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},//17
        {{0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},//18
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},//19

        //left
       {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},//20
       {{-0.5f,  0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},//21
       {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},//22
       {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}//23

   };

    std::vector<uint> indices = {
        0,3,1,    1,3,2,//front
        4,7,6,    4,5,7,//bottom
        8,9,10,   8,10,11,//back
        12,14,13, 14,15,13,//top
        16,17,18, 17,19,18,//right
        20,21,22,  21,23,22//left
    };
    ResourceRepository::addMesh("cube", Mesh(vertices, indices));

    return ResourceRepository::getMesh("cube");
}

MeshRef MeshBuilder::createSurfaceMesh() {
    std::vector<float> vertices;
    constexpr uint resolution = 20;
    glm::ivec2 size(1000);

    for (int i = 0; i < resolution-1; ++i) {
        for (int j = 0; j < resolution-1; ++j) {
            //top left
            vertices.push_back(-size.x/2.0f + size.x*i/(float)resolution);
            vertices.push_back(0.0f);
            vertices.push_back(-size.y/2.0f + size.y*j/(float)resolution);
            vertices.push_back(i / (float)resolution);
            vertices.push_back(j / (float)resolution);

            //bottom left
            vertices.push_back(-size.x/2.0f + size.x*i/(float)resolution);
            vertices.push_back(0.0f); // v.y
            vertices.push_back(-size.y/2.0f + size.y*(j+1)/(float)resolution);
            vertices.push_back(i / (float)resolution);
            vertices.push_back((j+1) / (float) resolution);

            //bottom right
            vertices.push_back(-size.x/2.0f + size.x*(i+1)/(float)resolution);
            vertices.push_back(0.0f);
            vertices.push_back(-size.y/2.0f + size.y*j/(float)resolution);
            vertices.push_back((i+1) / (float)resolution);
            vertices.push_back(j / (float)resolution);

            //top right
            vertices.push_back(-size.x/2.0f + size.x*(i+1)/(float)resolution);
            vertices.push_back(0.0f);
            vertices.push_back(-size.y/2.0f + size.y*(j+1)/(float)resolution);
            vertices.push_back((i+1) / (float)resolution);
            vertices.push_back((j+1) / (float)resolution);
        }
    }
    std::vector<uint> indices{};

    ResourceRepository::addMesh("surface", Mesh(vertices, indices));
    return ResourceRepository::getMesh("surface");
}
