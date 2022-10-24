#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include<bits/stdc++.h>
#include "model.hpp"
using namespace std;

#define PI 3.1428

void build_cuboid(vector<Vertex>& vertices, vector<int>& indices);

class Particle {

    public:
        Model* model;
        vec3 pos;
        vec3 vel;
        vec3 acc;
        bool dead;
        float time;
        float lifeSpan;
        Particle(Model* model, vec3 pos, vec3 vel, vec3 acc, float lifeSpan);
        void update(float dt);
        void update(vec3 newPos, vec3 newVel, vec3 newAcc);
        void draw(Shader* shader, Camera* cam);

};

Particle::Particle(Model* model, vec3 pos, vec3 vel, vec3 acc, float lifeSpan){
    this->model = model;
    this->pos = pos;
    this->vel = vel;
    this->acc = acc;
    this->dead = false;
    this->lifeSpan = lifeSpan;
}

void Particle::update(vec3 newPos, vec3 newVel, vec3 newAcc){
    if(this->dead){
        return;
    }
    if(this->lifeSpan < 0.0f){
        this->dead = true;
    }else{
        
        mat4 move = translate(mat4(1.0f), newPos - this->pos);
        this->pos = newPos;
        this->model->updateTransformByID(move, 0);
    }
}

void Particle::update(float dt){
    if(this->dead){
        return;
    }
    this->lifeSpan -= dt;
    this->time += dt;
    if(this->lifeSpan < 0.0f){
        this->dead = true;
    }else{
        this->vel += this->acc * dt;
        this->pos += this->vel * dt;
        mat4 move = translate(mat4(1.0f), this->vel * dt);
        this->model->updateTransformByID(move, 0);
    }
}

void Particle::draw(Shader* shader, Camera* cam){
    if(this->dead){
        return;
    }
    this->model->Draw(shader, cam);
}


#endif
