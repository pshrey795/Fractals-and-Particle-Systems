#ifndef PATH_HPP
#define PATH_HPP

#include "curve.hpp"

using namespace std;
using namespace glm;

class Path {

    public:
        vector<Curve*> curves; 
        float stepSize;
        int currentCurve;
        float currentParameterVal;
        vec3 currentPos;
        vec3 currentTangent;

        Path(float stepSize, vec3 startPos, vec3 startTangent){
            this->stepSize = stepSize;
            this->currentParameterVal = 0.0f;
            this->currentCurve = 0.0f;
            this->currentPos = startPos;
            this->currentTangent = startTangent;
        }
        void addCurve(Curve* curve){
            this->curves.push_back(curve);
        }
        pair<mat4,mat4> getNextPoint(){
            mat4 translateTransform = mat4(1.0f);
            mat4 rotateTransform = mat4(1.0f);
            if(abs(this->currentParameterVal - 1.0f) < 0.0001f){
                this->currentParameterVal = 0.0f;
                this->currentPos = this->curves[this->currentCurve]->b3;
                this->currentTangent = this->curves[this->currentCurve]->b3 - this->curves[this->currentCurve]->b2;
                this->currentCurve++;
                return make_pair(translateTransform, rotateTransform);
            }
            if(this->currentCurve >= this->curves.size()){
                return make_pair(translateTransform, rotateTransform);
            }
            float nextParameterVal = this->currentParameterVal + this->stepSize;
            vec3 nextPos = this->curves[this->currentCurve]->getPoint(nextParameterVal);
            vec3 nextTangent = nextPos - this->currentPos;
            int sign;
            vec3 crossProd = cross(this->currentTangent, nextTangent);
            float comp;
            vec3 rotAxis;
            switch(this->curves[this->currentCurve]->axis){
                case 0:{
                    comp = crossProd.y;
                    rotAxis = vec3(0.0f, 1.0f, 0.0f);
                    break;
                }
                case 1:{
                    comp = crossProd.z;
                    rotAxis = vec3(-1.0f, 0.0f, 0.0f);
                    break;
                }
                case 2:{
                    comp = crossProd.x;
                    rotAxis = vec3(0.0f, 0.0f, -1.0f);
                    break;
                }
            }
            if(comp == 0.0){
                sign = 0;
            }else{
                sign = comp > 0 ? 1 : -1;
            }
            float theta = acos(dot(normalize(this->currentTangent), normalize(nextTangent)));
            rotateTransform = rotate(rotateTransform, sign * theta, rotAxis);
            translateTransform = translate(translateTransform, nextPos - this->currentPos);
            this->currentPos = nextPos;
            this->currentTangent = nextTangent;
            
            this->currentParameterVal = nextParameterVal;
            return make_pair(translateTransform, rotateTransform);
        }


};

#endif
