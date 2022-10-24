#ifndef MARCH_HPP
#define MARCH_HPP

#include<bits/stdc++.h>
#include "particle.hpp"
using namespace std;

class marchSystem {
    public:
        int num1;
        int num2;
        vec3 baseVec1;
        vec3 baseVec2;
        vec3 base;
        float time;
        vector<Particle*> particles;
        marchSystem(vec3 base, vec3 baseVec1, vec3 baseVec2, int num1, int num2, float dist1, float dist2, float scale);
        void emit(int i);
        void update(float dt);
        void updateView(mat4 view);
        void updatePerspective(mat4 perspective);
        void draw(Shader* shader, Camera* cam);
};

marchSystem::marchSystem(vec3 base, vec3 baseVec1, vec3 baseVec2, int num1, int num2, float dist1, float dist2, float scaleVal){
    this->num1 = num1;
    this->num2 = num2;
    this->base = base;
    this->baseVec1 = baseVec1;
    this->baseVec2 = baseVec2;
    this->time = 0;
    vector<Vertex> vertices;
    vector<int> indices;
    build_cuboid(vertices,indices);
    for(int i=0;i<num1;i++){
        for(int j=0;j<num2;j++){
            Model* newParticle = new Model(0,0);
            newParticle->addShape(vertices, indices, BLUE);
            vec3 particlePos = base + i * dist1 * baseVec1 + j * dist2 * baseVec2;
            mat4 model = translate(mat4(1.0f), particlePos);
            model = scale(model, vec3(scaleVal));
            newParticle->setTransformByID(model, 0);
            vec3 newVel = vec3(0.0f);
            vec3 newAcc = vec3(0.0f);
            this->particles.push_back(new Particle(newParticle, particlePos, newVel, newAcc, 500));
        }
    }
}

void marchSystem::emit(int i){
    this->particles[i]->lifeSpan = 500;
    this->particles[i]->dead = false;
}

void marchSystem::updateView(mat4 view){
    for(int i=0;i<this->particles.size();i++){
        this->particles[i]->model->updateView(view);
    }
}

void marchSystem::update(float dt){
    this->time += dt;
    for(int i=0;i<this->particles.size();i++){
        this->particles[i]->lifeSpan -= dt;
        vec3 pos = this->particles[i]->pos;
        float u = dot(pos, this->baseVec1) + dt * 0.04f;
        float v = dot(pos, this->baseVec2);
        float w = sin(0.25 * time) / 5.0f;
        vec3 newVec = cross(baseVec1, baseVec2);
        vec3 newPos = u * this->baseVec1 + v * this->baseVec2 + w * newVec;
        vec3 newVel = vec3(0.0f);
        vec3 newAcc = vec3(0.0f);
        this->particles[i]->update(newPos, newVel, newAcc);
        if(this->particles[i]->dead){
            this->emit(i);
        }   
    }
}

void marchSystem::updatePerspective(mat4 perspective){
    for(int i=0;i<this->particles.size();i++){
        this->particles[i]->model->updatePerspective(perspective);
    }
}

void marchSystem::draw(Shader* shader, Camera* cam){
    for(int i=0;i<this->particles.size();i++){
        this->particles[i]->draw(shader, cam);
    }
}


#endif