#include "../../../includes/glad.h"
#include "../../resources/Texture.hpp"
#include "Cube.hpp"

uint Cube::VAO, Cube::VBO, Cube::EBO;

void Cube::load(){
    if (VAO == 0) {
        float vertices[] = {
        //vertices          //normals         //tex coords

        //front
         0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,//0
         0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,//1
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,//2
        -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,//3

        //bottom
         0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,//4
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,//5
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,//6
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,//7

        //back
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,//8
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,//9
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,//10
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,//11

        //top
         0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,//12
        -0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,//13
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,//14
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,//15

        //right
         0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,//16
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//17
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//18
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,//19

         //left
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,//20
        -0.5f,  0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//21
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//22
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,//23

    };
    uint indices[] = {
        0,3,1,    1,3,2,//front
        4,7,6,    4,5,7,//bottom
        8,9,10,   8,10,11,//back
        12,14,13, 14,15,13,//top
        16,17,18, 17,19,18,//right
        20,21,22,  21,23,22//left
    };
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //1. Vertex attribute to configure, 2. number of values for this atttribute
    //5. Stride, 6. offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);//vertex

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);//color

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);//texcoords
    }
    glBindVertexArray(VAO);

    
}

void Cube::draw(const Shader & shader) const {
    load();
    setUniforms(shader, 0);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Cube::free() {
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Cube::draw(){
    load();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Cube::drawSkyBox(const Shader & shader, const Texture & skyboxTex, const std::string & samplerName) {
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_FRONT);
    shader.use();
    skyboxTex.bind(shader, samplerName, 0);
    draw();
    glCullFace(GL_BACK);
    glDepthFunc(GL_LESS);
}
