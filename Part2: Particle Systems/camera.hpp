#ifndef CAMERA_HPP
#define CAMERA_HPP

#include<bits/stdc++.h>
#include "glad/glad.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Camera{
    public:
        vec3 pos;
        vec3 target;
        vec3 front;
        vec3 up;
        mat4 transform;

        Camera(vec3 pos, vec3 up, vec3 target){
            this->pos = pos;
            this->target = target;
            this->up = up;
            this->transform = mat4(1.0f);
        }

        void transformCamera(mat4 t){
            this->transform = this->transform * t;
        }

        void changeCamera(mat4 t){
            this->transform = t * this->transform;
        }

        mat4 getViewMatrix(){
            return lookAt(vec3(transform * vec4(pos,1.0)),vec3(transform * vec4(target,1.0)),mat3(transpose(inverse(transform))) * up);
        }
};



#endif