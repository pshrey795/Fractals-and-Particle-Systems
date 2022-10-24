#ifndef SHAPE_HPP
#define SHAPE_HPP

#include<bits/stdc++.h>
#include "glad/glad.h"
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include "shader.hpp"
#include "camera.hpp"

//Adding image loader header file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
using namespace glm;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float spec_coeff;

    Material(){
        
    }

    Material(vec3 ambient, vec3 diffuse, vec3 specular, float spec_coeff){
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->spec_coeff = spec_coeff;
    }
};

enum Material_Type{
    METAL, PLASTIC, RUBBER, GROUND, BUILDING1, BUILDING2, BUILDING3, ARCH, BUILDING4, BROWN, GRASS, BLADE, BLUE, ORANGE, GREEN, WHITE
};

struct Vertex{
    vec3 Position;
    vec3 Normal;

    Vertex(vec3 pos, vec3 norm){
        this->Position = pos;
        this->Normal = norm;
    }
};

struct Transform{
    mat4 model;
    mat4 view;
    mat4 projection;

    Transform(){
        model = mat4(1.0f);
        view = mat4(1.0f);
        projection = mat4(1.0f);
    }
};

class Shape {
    public:
        vector<Vertex> vertices;
        vector<int> faces;
        struct Transform transform;
        mat4 localTransform;
        struct Material material;
        Shape(vector<Vertex> vertices, vector<int> faces, Material_Type m);
        void Draw(Shader* shader, Camera* cam);
        void changeTransform(mat4 t);
        void updateTransform(mat4 t);
        void setTransform(mat4 t);

    private:
        unsigned int VAO, VBO, EBO;
        void createShape();
        void processMaterial(Material_Type m);
};

Shape::Shape(vector<Vertex> vertices, vector<int> faces, Material_Type m) {
    this->vertices = vertices;
    this->faces = faces;
    this->transform = Transform();
    this->material = Material();
    this->localTransform = mat4(1.0f);
    processMaterial(m);
    createShape();
}

void Shape::changeTransform(mat4 t){
    transform.model = t * transform.model;
}

void Shape::updateTransform(mat4 t){
    transform.model = transform.model * t;
}

void Shape::setTransform(mat4 t){
    this->localTransform = t;
}

void Shape::createShape(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //Vertex Array
    glBindVertexArray(VAO);
    //Vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    //Faces/Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(int), &faces[0], GL_STATIC_DRAW);
    // Vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Shape::processMaterial(Material_Type m){
    switch(m){
        case METAL: {
            this->material = Material(vec3(0.2125f, 0.1275f, 0.054f), vec3(0.714, 0.4284f, 0.18144f), vec3(0.393548f, 0.271906f, 0.166721f), 32.0f);
            break;
        }
        case PLASTIC: {
            this->material = Material(vec3(0.0f, 0.0f, 0.0f), vec3(0.1f, 0.35f, 0.1f), vec3(0.45f, 0.55f, 0.45f), 10.0f);
            break;
        }
        case RUBBER: {
            this->material = Material(vec3(0.05f, 0.0f, 1.0f), vec3(0.5f, 0.4f, 1.0f), vec3(1.0f, 0.04f, 0.04f), 10.0f);
            break;
        }
        case GROUND: {
            this->material = Material(vec3(0.05f, 0.05f, 0.0f), vec3(0.5f, 0.5f, 0.4f), vec3(0.0f, 0.0f, 0.0f), 10.0f);
            break;
        }

        case BUILDING1: {
            this->material = Material(vec3(0.19225f, 0.19225f, 0.19225f), vec3(0.50754f, 0.50754f, 0.50754f), vec3(0.508273f, 0.508273f, 0.508273f), 10.0f);
            break;
        }

        case BUILDING2: {
            this->material = Material(vec3(0.1745f, 0.1175f, 0.1175f), vec3(0.61424f, 0.04136f, 0.4136f), vec3(0.727811f, 0.626959f, 0.626959f), 10.0f);
            break;
        }

        case ARCH: {
            this->material = Material(vec3(0.85f, 0.53f, 0.1f), vec3(0.85f, 0.53f, 0.1f), vec3(0.01f, 0.01f, 0.01f), 10.0f);
            break;
        }

        case BUILDING3: {
            this->material = Material(vec3(0.36f, 0.20f, 0.1f), vec3(0.36f, 0.20f, 0.1f), vec3(0.01f, 0.01f, 0.01f), 10.0f);
            break;
        }

        case BUILDING4: {
            this->material = Material(vec3(0.735f, 0.2225f, 0.1575f), vec3(0.54f, 0.89f, 0.63f), vec3(0.716228f, 0.316228f, 0.316228f), 10.0f);
            break;
        }

        case BROWN: {
            this->material = Material(vec3(0.2f,0.1f,0.1f), vec3(0.2f,0.1f,0.1f), vec3(0.01f,0.01f,0.01f), 10.0f);
            break;
        }

        case GRASS: {
            this->material = Material(vec3(0.1f,0.5f,0.1f), vec3(0.1f,0.5f,0.1f), vec3(0.01f,0.01f,0.01f), 10.0f);
            break;
        }

        case BLADE: {
            this->material = Material(vec3(0.19225f,0.19225f,0.19225f), vec3(0.50754f,0.50754f,0.50754f), vec3(0.508273f,0.508273f,0.508273f), 64.0f);
            break;
        } 

        case BLUE: {
            this->material = Material(vec3(0.3f,0.3f,0.6f), vec3(0.3f,0.3f,0.6f), vec3(0.8f,0.8f,0.8f), 64.0f);
            break;
        } 

        case ORANGE: {
            this->material = Material(vec3(1.0f,0.5f,0.0f), vec3(1.0f,0.5f,0.0f), vec3(0.8f,0.8f,0.8f), 64.0f);
            break;
        } 

        case GREEN: {
            this->material = Material(vec3(0.3f,1.0f,0.3f), vec3(0.3f,1.0f,0.3f), vec3(0.8f,0.8f,0.8f), 64.0f);
            break;
        } 

        case WHITE: {
            this->material = Material(vec3(1.0f,1.0f,1.0f), vec3(1.0f,1.0f,1.0f), vec3(0.8f,0.8f,0.8f), 64.0f);
            break;
        } 
    }
}

void Shape::Draw(Shader* shader, Camera* cam){
    shader->use();
    shader->setMat4("model", transform.model * localTransform);
    shader->setMat4("view", transform.view);
    shader->setMat4("projection", transform.projection);
    shader->setMat3("normalMatrix", mat3(transpose(inverse(transform.model))));
    shader->setVec3("material.ambient", this->material.ambient);
    shader->setVec3("material.diffuse", this->material.diffuse);
    shader->setVec3("material.specular", this->material.specular);
    shader->setFloat("material.spec_coeff", this->material.spec_coeff);
    // shader->setVec3("point_light.position", vec3(100.0f, 100.0f, 100.0f));
    shader->setVec3("sun.direction", vec3(-1.0,-1.0,-2.0));
    shader->setVec3("sun.ambient", vec3(1.0f, 1.0f, 1.0f));
    shader->setVec3("sun.diffuse", vec3(1.0f, 1.0f, 1.0f));
    shader->setVec3("sun.specular", vec3(1.0f, 1.0f, 1.0f));
    shader->setVec3("viewPos", cam->pos);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


#endif