#ifndef FRACTAL_HPP
#define FRACTAL_HPP

#include "util.hpp"
#include<bits/stdc++.h>
using namespace std;

//Terrain generation using Triangle Edge Subdivision
float random_noise(float epsilon, float delta){
     float r = (float)(rand() - (float)RAND_MAX/2)/RAND_MAX;  // r between -0.5 and 0.5
     return (r + delta) * epsilon; //Noise adjustment
}

void build_fractal_terrain(vector<Vertex>& vertices, Triangle base_triangle, vector<vec2> base_triangle_textures, float epsilon, float delta, int depth){
       if (depth<15){
              vec3 a=base_triangle.vertex1;
              vec3 b=base_triangle.vertex2;
              vec3 c=base_triangle.vertex3;
              vec2 at = base_triangle_textures[0];
              vec2 bt = base_triangle_textures[1];
              vec2 ct = base_triangle_textures[2];
              vec3 n=base_triangle.normal;
              
              vec3 d = vec3((a.x + b.x)/2, (a.y + b.y)/2 + random_noise(epsilon, delta),(a.z + b.z)/2);
              vec2 dt = vec2((at.y + bt.x)/2, (at.y + bt.y)/2);
              vec3 e = vec3((c.x + b.x)/2, (c.y + b.y)/2 + random_noise(epsilon, delta),(c.z + b.z)/2);
              vec2 et = vec2((ct.x + bt.x)/2, (ct.y + bt.y)/2);
              vec3 f = vec3((a.x + c.x)/2, (a.y + c.y)/2 + random_noise(epsilon, delta),(a.z + c.z)/2);
              vec2 ft = vec2((at.x + ct.x)/2, (at.y + ct.y)/2);

              Triangle sub_triangle1(d,e,f,cross(e-d,f-d));
              vector<vec2> sub_triangle1_textures = {dt,et,ft};
              Triangle sub_triangle2(e,c,f,cross(c-e,f-e));
              vector<vec2> sub_triangle2_textures = {et,ct,ft};
              Triangle sub_triangle3(f,a,d,cross(a-f,d-f));
              vector<vec2> sub_triangle3_textures = {ft,at,dt};
              Triangle sub_triangle4(d,b,e,cross(e-b,d-b));
              vector<vec2> sub_triangle4_textures = {dt,bt,et};

              vertices.push_back(Vertex(d, cross(e-d,f-d), dt));
              vertices.push_back(Vertex(e, cross(e-d,f-d), et));
              vertices.push_back(Vertex(f, cross(e-d,f-d), ft));

              vertices.push_back(Vertex(e, cross(c-e,f-e), et));
              vertices.push_back(Vertex(c, cross(c-e,f-e), ct));
              vertices.push_back(Vertex(f, cross(c-e,f-e), ft));

              vertices.push_back(Vertex(f, cross(a-f,d-f), ft));
              vertices.push_back(Vertex(a, cross(a-f,d-f), at));
              vertices.push_back(Vertex(d, cross(a-f,d-f), dt));

              vertices.push_back(Vertex(d, cross(e-b,d-b), dt));
              vertices.push_back(Vertex(b, cross(e-b,d-b), bt));
              vertices.push_back(Vertex(e, cross(e-b,d-b), et));

              vertices.push_back(Vertex(b, cross(b-e,c-e)));
              vertices.push_back(Vertex(c, cross(b-e,c-e)));
              vertices.push_back(Vertex(e, cross(b-e,c-e)));

              vertices.push_back(Vertex(c, cross(c-f,a-f)));
              vertices.push_back(Vertex(a, cross(c-f,a-f)));
              vertices.push_back(Vertex(f, cross(c-f,a-f)));

              vertices.push_back(Vertex(a, cross(a-d,b-d)));
              vertices.push_back(Vertex(b, cross(a-d,b-d)));
              vertices.push_back(Vertex(d, cross(a-d,b-d)));

              if (dot(sub_triangle1.normal,vec3(0.0f,1.0f,0.0f))>0.5){
                     build_fractal_terrain(vertices, sub_triangle1, sub_triangle1_textures, epsilon, delta, depth+1);        
              }
              if (dot(sub_triangle2.normal,vec3(0.0f,1.0f,0.0f))>0.5){
                     build_fractal_terrain(vertices, sub_triangle2, sub_triangle2_textures, epsilon, delta, depth+1);
              }
              if (dot(sub_triangle3.normal,vec3(0.0f,1.0f,0.0f))>0.5){
                     build_fractal_terrain(vertices, sub_triangle3, sub_triangle3_textures, epsilon, delta, depth+1);
              }
              if (dot(sub_triangle4.normal,vec3(0.0f,1.0f,0.0f))>0.5){
                     build_fractal_terrain(vertices, sub_triangle4, sub_triangle4_textures, epsilon, delta, depth+1);
              }
       }
}

void make_terrain_row1(int i, int j){
       vector<vec2> base_triangle_textures = {
              vec2(0.0f,0.0f),
              vec2(3.5f,0.5f),
              vec2(0.5f,3.5f)
       };
       float edge=80.f;
       Triangle base_triangle1(vec3(-edge+(2*edge*j),0.0f,edge+(2*edge*i)),vec3(edge+(2*edge*j),0.0f,-edge+(2*edge*i)),vec3(-edge+(2*edge*j),0.0f,-edge+(2*edge*i)),vec3(0.0f,1.0f,0.0f));
       Triangle base_triangle2(vec3(-edge+(2*edge*j),0.0f,edge+(2*edge*i)),vec3(edge+(2*edge*j),0.0f,edge+(2*edge*i)),vec3(edge+(2*edge*j),0.0f,-edge+(2*edge*i)),vec3(0.0f,1.0f,0.0f));

       vector<Vertex> vertices_triangle1;
       vector<int> indices_triangle1;
       build_triangle(vertices_triangle1,indices_triangle1,base_triangle1);
       build_fractal_terrain(vertices_triangle1, base_triangle1, base_triangle_textures, 1.5, 0.45, 0);
       for(int i = 0;i<vertices_triangle1.size();i++){
              if(i<3)
                     continue;
              else
                     indices_triangle1.push_back(i);
       }
       Model* newTerrain1 = new Model(0,0);
       newTerrain1->addShape(vertices_triangle1,indices_triangle1, TEXTURE);
       newTerrain1->shapes[0]->addTexture("/home/shrey/Shrey/IITD/Sem6/Computer Graphics/Computer-Graphics-Assignments/Assignment 3: Terrain Modelling/Part1: Fractal Terrain Modelling/assets/mountains.jpg");
       mat4 model = mat4(1.0f);
       newTerrain1->setTransformByID(model,0);

       vector<Vertex> vertices_triangle2;
       vector<int> indices_triangle2;
       build_triangle(vertices_triangle2,indices_triangle2,base_triangle2);
       build_fractal_terrain(vertices_triangle2, base_triangle2, base_triangle_textures, 1.5, 0.45, 0);
       for(int i = 0;i<vertices_triangle2.size();i++){
              if(i<3)
              continue;
              else
              indices_triangle2.push_back(i);
       }
       Model* newTerrain2 = new Model(0,0);
       newTerrain2->addShape(vertices_triangle2,indices_triangle2, TEXTURE);
       newTerrain2->shapes[0]->addTexture("/home/shrey/Shrey/IITD/Sem6/Computer Graphics/Computer-Graphics-Assignments/Assignment 3: Terrain Modelling/Part1: Fractal Terrain Modelling/assets/mountains.jpg");
       model = mat4(1.0f);
       newTerrain2->setTransformByID(model,0);

       terrain1.push_back(newTerrain1);
       terrain1.push_back(newTerrain2);
}

void make_terrain_row2(int i, int j){
       vector<vec2> base_triangle_textures = {
              vec2(0.0f,0.0f),
              vec2(3.5f,0.5f),
              vec2(0.5f,3.5f)
       };
       float edge=80.f;
       Triangle base_triangle1(vec3(-edge+(2*edge*j),0.0f,edge+(2*edge*i)),vec3(edge+(2*edge*j),0.0f,-edge+(2*edge*i)),vec3(-edge+(2*edge*j),0.0f,-edge+(2*edge*i)),vec3(0.0f,1.0f,0.0f));
       Triangle base_triangle2(vec3(-edge+(2*edge*j),0.0f,edge+(2*edge*i)),vec3(edge+(2*edge*j),0.0f,edge+(2*edge*i)),vec3(edge+(2*edge*j),0.0f,-edge+(2*edge*i)),vec3(0.0f,1.0f,0.0f));

       vector<Vertex> vertices_triangle1;
       vector<int> indices_triangle1;
       build_triangle(vertices_triangle1,indices_triangle1,base_triangle1);
       build_fractal_terrain(vertices_triangle1, base_triangle1, base_triangle_textures, 1.5, 0.45, 0);
       for(int i = 0;i<vertices_triangle1.size();i++){
              if(i<3)
                     continue;
              else
                     indices_triangle1.push_back(i);
       }
       Model* newTerrain1 = new Model(0,0);
       newTerrain1->addShape(vertices_triangle1,indices_triangle1, TEXTURE);
       newTerrain1->shapes[0]->addTexture("/home/shrey/Shrey/IITD/Sem6/Computer Graphics/Computer-Graphics-Assignments/Assignment 3: Terrain Modelling/Part1: Fractal Terrain Modelling/assets/mountains.jpg");
       mat4 model = mat4(1.0f);
       newTerrain1->setTransformByID(model,0);

       vector<Vertex> vertices_triangle2;
       vector<int> indices_triangle2;
       build_triangle(vertices_triangle2,indices_triangle2,base_triangle2);
       build_fractal_terrain(vertices_triangle2, base_triangle2, base_triangle_textures, 1.5, 0.45, 0);
       for(int i = 0;i<vertices_triangle2.size();i++){
              if(i<3)
              continue;
              else
              indices_triangle2.push_back(i);
       }
       Model* newTerrain2 = new Model(0,0);
       newTerrain2->addShape(vertices_triangle2,indices_triangle2, TEXTURE);
       newTerrain2->shapes[0]->addTexture("/home/shrey/Shrey/IITD/Sem6/Computer Graphics/Computer-Graphics-Assignments/Assignment 3: Terrain Modelling/Part1: Fractal Terrain Modelling/assets/mountains.jpg");
       model = mat4(1.0f);
       newTerrain2->setTransformByID(model,0);

       terrain2.push_back(newTerrain1);
       terrain2.push_back(newTerrain2);
}


//Tree generation using L systems
string generateTreeString(int depth){
       if(depth == 0){
              return "F";
       }else{
              string curr = generateTreeString(depth-1);
              string next = "";
              for(int i=0;i<curr.length();i++){
                     if(curr[i] == 'F'){
                            next += "F[-F][+F]";
                     }else{
                            next += curr[i];
                     }
              }
              return next;
       }
}

//Rendering tree using L system generated string
Model* createTree(string treeString, float theta){
       stack<mat4> transformStack;
       stack<Model*> parentStack; 
       mat4 currentTransform = mat4(1.0f);
       Model* treeModel;
       Model* currentModel;
       int currentDepth = 0;
       int currentIndex = 0;
       vector<Vertex> vertices_trunk;
       vector<int> indices_trunk;
       build_cylinder(vertices_trunk,indices_trunk);
       mat4 initialTransform = translate(mat4(1.0f), vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f)) * scale(mat4(1.0f), vec3(1.0f, 0.2f, 0.2f));

       vector<Vertex> vertices_leaves;
       vector<int> indices_leaves;
       build_flower(vertices_leaves,indices_leaves);

       for(int i=0;i<treeString.size();i++){
              if(treeString[i] == 'F'){
                     if(parentStack.size()==0){
                            treeModel = new Model(currentIndex,currentDepth);
                            treeModel->addShape(vertices_trunk,indices_trunk,BROWN);
                            treeModel->setTransformByID(initialTransform,currentIndex);
                            currentModel = treeModel;
                     }else{
                            Model* newModel = new Model(currentIndex, currentDepth);
                            newModel->addShape(vertices_trunk, indices_trunk, BROWN);
                            mat4 newTransform = currentTransform * initialTransform;
                            newModel->setTransformByID(newTransform, currentIndex);
                            currentModel = newModel;
                            parentStack.top()->addChildModel(newModel);
                     }
                     currentIndex++;
                     currentTransform = currentTransform * translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f));
              }else if(treeString[i] == '+'){
                     currentTransform = currentTransform * rotate(mat4(1.0f), radians(theta), vec3(-0.5f, 0.0f, 0.816f)); 
              }else if(treeString[i] == '-'){
                     currentTransform = currentTransform * rotate(mat4(1.0f), radians(theta), vec3(1.0f, 0.0f, 0.0f));
              }else if(treeString[i] == '*'){
                     currentTransform = currentTransform * rotate(mat4(1.0f), radians(theta), vec3(-0.5f, 0.0f, -0.816f));
              }else if(treeString[i] == '['){
                     currentDepth++;
                     parentStack.push(currentModel);
                     transformStack.push(currentTransform);
              }else if (treeString[i] == ']'){
                     Model* newModel = new Model(currentIndex,currentDepth+1);
                     newModel->addShape(vertices_leaves,indices_leaves,PLASTIC);
                     newModel->setTransformByID(currentTransform,currentIndex);
                     currentModel->addChildModel(newModel);
                     assert(parentStack.size() > 0 && transformStack.size() > 0);
                     currentDepth--;
                     currentModel = parentStack.top();
                     parentStack.pop();
                     currentTransform = transformStack.top();
                     transformStack.pop();
              }
       }
       return treeModel;
}

#endif
