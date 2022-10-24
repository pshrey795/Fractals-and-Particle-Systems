#ifndef SHADER_HPP
#define SHADER_HPP

#include<bits/stdc++.h>
#include "glad/glad.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
using namespace std;
using namespace glm;

class Shader{
    public:
        unsigned int ID;
        Shader(string vertexPath, string fragmentPath);
        void use();
        void setBool(const string &name, bool value);
        void setInt(const string &name, int value);
        void setFloat(const string &name, float value);
        void setVec3(const string &name, vec3 value);
        void setMat3(const string &name, mat3 value);
        void setMat4(const string &name, mat4 value);
        void deleteShader();

};

Shader::Shader(string vertexPath, string fragmentPath){
    // 1. retrieve the vertex/fragment source code from filePath
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }catch(ifstream::failure e){
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();


    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    //Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment,1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragment,512,NULL,infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    //Shader Program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    glLinkProgram(this->ID);

    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(this->ID,512,NULL,infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    //Delete Shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader::use(){
    glUseProgram(ID);
}

void Shader::setBool(const string &name, bool value){
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const string &name, int value){
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const string &name, float value){
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const string &name, vec3 value){
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value_ptr(value));
}

void Shader::setMat3(const string &name, mat3 value){
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()),1, GL_FALSE, value_ptr(value));
}

void Shader::setMat4(const string &name, mat4 value){
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value_ptr(value));
}

void Shader::deleteShader(){
    glDeleteProgram(ID);
}

#endif