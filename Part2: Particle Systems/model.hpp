#ifndef MODEL_HPP
#define MODEL_HPP

#include "shape.hpp"

using namespace std;
using namespace glm;
using namespace Assimp;

class Model {

    public:
        vector<Shape*> shapes;
        vector<Model*> childModels;
        Camera* cam;
        int ID;
        int depth;
        float fanSpeed; 
        bool firstPerson;
        float movementSpeed;

        Model(int ID, int depth);
        void setCamera(Camera* cam);
        void setFanSpeed(float fanSpeed, vector<int> fans);
        void setMoveSpeed(float speed);
        void setFirstPerson();
        void addShape(vector<Vertex> vertices, vector<int> faces, Material_Type m);
        void addChildModel(Model* m);
        void changeTransformByDepth(mat4 t, int minDepth);
        void changeTransformByID(mat4 t, int ID);
        void updateTransformByDepth(mat4 t, int minDepth);
        void updateTransformByID(mat4 t, int newID);
        void setTransformByDepth(mat4 t, int minDepth);
        void setTransformByID(mat4 t, int newID);
        void updateView(mat4 v);
        void updatePerspective(mat4 p);
        void Draw(Shader* shader);
        void Draw(Shader* shader, Camera* cam);

    private:

};

Model::Model(int ID, int depth){
    this->depth = depth;
    this->ID = ID;
    this->firstPerson = false;
    this->fanSpeed = 10.0f;
    this->movementSpeed = 2.0f;
}

void Model::setCamera(Camera* cam){
    this->cam = cam;
}

void Model::setFanSpeed(float fanSpeed, vector<int> fans){
    if(this->depth == 3){
        for(int i=0;i<fans.size();i++){
            if(fans[i] == this->ID){
                this->fanSpeed = fanSpeed;
            }
        }
    }
    for(int i=0;i<childModels.size();i++){
        childModels[i]->setFanSpeed(fanSpeed, fans);
    }
}

void Model::setMoveSpeed(float speed){
    this->movementSpeed = speed;
}

void Model::setFirstPerson(){
    this->firstPerson = true;
}

//This indirect constructor is for building the model from explicit co-ordinates and faces
void Model::addShape(vector<Vertex> vertices, vector<int> faces, Material_Type m) {
    shapes.push_back(new Shape(vertices, faces, m));
}

void Model::Draw(Shader* shader) {
    for(int i = 0; i < shapes.size(); i++) {
        shapes[i]->Draw(shader,this->cam);
    }
    for (int i = 0; i < childModels.size(); i++) {
        childModels[i]->Draw(shader,this->cam);
    }
}

void Model::Draw(Shader* shader, Camera* cam) {
    if(this->depth==3){
        int sign;
        switch(this->ID){
            case 9:{
                sign = 1;
                break;    
            }
            case 10:{
                sign = -1;
                break;    
            }
            case 11:{
                sign = -1;
                break;    
            }
            case 12:{
                sign = 1;
                break;    
            }
            default:{
                sign = 0;
                break;
            }
        }
        for(int i=0;i<shapes.size();i++){
            shapes[i]->localTransform = shapes[i]->localTransform * rotate(mat4(1.0f), radians(sign * this->fanSpeed * 2.0f), vec3(0.0f, 1.0f, 0.0f));
        }
    }
    for(int i = 0; i < shapes.size(); i++) {
        shapes[i]->Draw(shader,cam);
    }
    for (int i = 0; i < childModels.size(); i++) {
        childModels[i]->Draw(shader,cam);
    }
}

void Model::addChildModel(Model* m) {
    childModels.push_back(m);
}

void Model::updateView(mat4 v){
    for(int i = 0; i < shapes.size(); i++) {
        shapes[i]->transform.view = v;
    }
    for (int i = 0; i < childModels.size(); i++) {
        childModels[i]->updateView(v);
    }
}

void Model::updatePerspective(mat4 p){
    for(int i = 0; i < shapes.size(); i++) {
        shapes[i]->transform.projection = p;
    }
    for (int i = 0; i < childModels.size(); i++) {
        childModels[i]->updatePerspective(p);
    }
}

void Model::changeTransformByID(mat4 t, int newID){
    if(this->ID == newID){
        for(int i=0;i<shapes.size();i++){
            shapes[i]->changeTransform(t);
        }
    }else{
        for(int i=0;i<childModels.size();i++){
            childModels[i]->changeTransformByID(t,newID);
        }
    }
}

void Model::updateTransformByID(mat4 t, int newID){
    if(this->ID == newID){
        for(int i=0;i<shapes.size();i++){
            shapes[i]->updateTransform(t);
        }
    }else{
        for(int i=0;i<childModels.size();i++){
            childModels[i]->updateTransformByID(t,newID);
        }
    }
}

void Model::setTransformByID(mat4 t, int newID){
    if(this->ID == newID){
        for(int i=0;i<shapes.size();i++){
            shapes[i]->setTransform(t);
        }
    }else{
        for(int i=0;i<childModels.size();i++){
            childModels[i]->setTransformByID(t,newID);
        }
    }
}

//newDepth is the least depth at which to start applying any transformation
void Model::changeTransformByDepth(mat4 t, int minDepth){
    if(this->depth >= minDepth){
        if(this->firstPerson){
            this->cam->changeCamera(t);
        }
        for(int i=0;i<shapes.size();i++){
            shapes[i]->changeTransform(t);
        }
    }
    for(int i=0;i<childModels.size();i++){
        childModels[i]->changeTransformByDepth(t,minDepth);
    }
}

void Model::updateTransformByDepth(mat4 t, int minDepth){
    if(this->depth >= minDepth){
        if(this->firstPerson){
            this->cam->transformCamera(t);
        }
        for(int i=0;i<shapes.size();i++){
            shapes[i]->updateTransform(t);
        }
    }
    for(int i=0;i<childModels.size();i++){
        childModels[i]->updateTransformByDepth(t,minDepth);
    }
}

void Model::setTransformByDepth(mat4 t, int minDepth){
    if(this->depth >= minDepth){
        for(int i=0;i<shapes.size();i++){
            shapes[i]->setTransform(t);
        }
    }
    for(int i=0;i<childModels.size();i++){
        childModels[i]->setTransformByDepth(t,minDepth);
    }
}



#endif