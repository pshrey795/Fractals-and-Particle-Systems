#ifndef UTIL_HPP
#define UTIL_HPP

#include "model.hpp"
#include "camera.hpp"
#include "path.hpp"
#include "flagsys.hpp"
#include "marchsys.hpp"

using namespace std;
using namespace glm;

//Global variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float yawVal = -90.0f;
float pitchVal = 0.0f;
float lastX = 800.0f;
float lastY = 600.0f;
float fov = 45.0f;

//Camera parameters
vec3 cameraPos1 = vec3(20.0f, 10.0f, 20.0f);
vec3 cameraTop1 = vec3(-1.0f, 4.0f, -1.0f);
vec3 cameraTarget1 = vec3(0.0f, 0.0f, 0.0f);

//Drone objects
Model *drone1;
float time1;

//For debugging
vec3 startPoint = vec3(0.0f, 0.0f, 0.0f);

//Handling Input
void rotateDrones(vec3 axis){
    mat4 rotation = rotate(mat4(1.0f),radians(drone1->movementSpeed),axis);
    drone1->updateTransformByDepth(rotation, 0);
}

void translateDrones(vec3 axis){
    mat4 translation = translate(mat4(1.0f),0.1f * drone1->movementSpeed * axis);
    drone1->updateTransformByDepth(translation, 0);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }else if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        rotateDrones(vec3(1.0,0.0,0.0));
    }else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        rotateDrones(vec3(-1.0,0.0,0.0));
    }else if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        rotateDrones(vec3(0.0,1.0,0.0));
    }else if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        rotateDrones(vec3(0.0,-1.0,0.0));
    }else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        rotateDrones(vec3(0.0,0.0,+1.0));
    }else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        rotateDrones(vec3(0.0,0.0,-1.0));
    }else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        translateDrones(vec3(1.0,0.0,0.0));
    }else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        translateDrones(vec3(-1.0,0.0,0.0));
    }else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        translateDrones(vec3(0.0,0.0,1.0));
    }else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        translateDrones(vec3(0.0,0.0,-1.0));
    }else if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
        translateDrones(vec3(0.0,-1.0,0.0));
    }else if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
        translateDrones(vec3(0.0,1.0,0.0));
    }

}   

//Window related auxiliary functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

//shapes

void build_circle(vector<Vertex>& vertices, vector<int>& indices){
    int n=360;

    for (int i=0;i<n;i++){
        double theta= (2*(M_PI)*i)/n;
        double theta_nxt= (2*(M_PI)*(i+1))/n;
        vertices.push_back(Vertex(vec3(0.0f, 0.0f, 0.0),  vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(vec3(-sin(theta), 0.0f, cos(theta)),  vec3(0.0f, 1.0f, 0.0f)));
        vertices.push_back(Vertex(vec3(-sin(theta_nxt), 0.0f, cos(theta_nxt)),  vec3(0.0f, 1.0f, 0.0f)));
    }

    for (int i=0;i<3*n;i++){
        indices.push_back(i);
    }
}

void build_boundary(vector<Vertex>& vertices, vector<int>& indices){
    int n=360;

    for (int i=0;i<n;i++){
        double theta= (2*(M_PI)*i)/n;
        double theta_nxt= (2*(M_PI)*(i+1))/n;

        vec3 normal= vec3((cos(theta_nxt)-cos(theta)), 0,  (sin(theta_nxt)-sin(theta)));
        vertices.push_back(Vertex(vec3(-sin(theta), -1.0f, cos(theta)),  normal));
        vertices.push_back(Vertex(vec3(-sin(theta), 1.0f , cos(theta)),  normal));
        vertices.push_back(Vertex(vec3(-sin(theta_nxt), 1.0f, cos(theta_nxt)),  normal));
        
        vertices.push_back(Vertex(vec3(-sin(theta), -1.0f, cos(theta)),  normal));
        vertices.push_back(Vertex(vec3(-sin(theta_nxt), 1.0f, cos(theta_nxt)),  normal));
        vertices.push_back(Vertex(vec3(-sin(theta_nxt), -1.0f, cos(theta_nxt)),  normal));
    }

    for (int i=0;i<6*n;i++){
        indices.push_back(i);
    }
}

void build_cuboid(vector<Vertex>& vertices, vector<int>& indices){
    vertices = {
        Vertex(vec3(-0.5f, -0.5f, -0.5f),  vec3(0.0f,  0.0f, -1.0f)), 
        Vertex(vec3(0.5f, -0.5f, -0.5f),  vec3(0.0f,  0.0f, -1.0f)),
        Vertex(vec3(0.5f, 0.5f, -0.5f),  vec3(0.0f,  0.0f, -1.0f)),
        Vertex(vec3(0.5f, 0.5f, -0.5f),  vec3(0.0f,  0.0f, -1.0f)),
        Vertex(vec3(-0.5f, 0.5f, -0.5f),  vec3(0.0f,  0.0f, -1.0f)),
        Vertex(vec3(-0.5f, -0.5f, -0.5f),  vec3(0.0f,  0.0f, -1.0f)),

        Vertex(vec3(-0.5f, -0.5f, 0.5f),  vec3(0.0f,  0.0f, 1.0f)), 
        Vertex(vec3(0.5f, -0.5f, 0.5f),  vec3(0.0f,  0.0f, 1.0f)),
        Vertex(vec3(0.5f, 0.5f, 0.5f),  vec3(0.0f,  0.0f, 1.0f)),
        Vertex(vec3(0.5f, 0.5f, 0.5f),  vec3(0.0f,  0.0f, 1.0f)),
        Vertex(vec3(-0.5f, 0.5f, 0.5f),  vec3(0.0f,  0.0f, 1.0f)),
        Vertex(vec3(-0.5f, -0.5f, 0.5f),  vec3(0.0f,  0.0f, 1.0f)),  

        Vertex(vec3(-0.5f, 0.5f, 0.5f),  vec3(-1.0f,  0.0f, 0.0f)), 
        Vertex(vec3(-0.5f, 0.5f, -0.5f),  vec3(-1.0f,  0.0f, 0.0f)),
        Vertex(vec3(-0.5f, -0.5f, -0.5f),  vec3(-1.0f,  0.0f, 0.0f)),
        Vertex(vec3(-0.5f, -0.5f, -0.5f),  vec3(-1.0f,  0.0f, 0.0f)),
        Vertex(vec3(-0.5f, -0.5f, 0.5f),  vec3(-1.0f,  0.0f, 0.0f)),
        Vertex(vec3(-0.5f, 0.5f, 0.5f),  vec3(-1.0f,  0.0f, 0.0f)),   

        Vertex(vec3(0.5f, 0.5f, 0.5f),  vec3(1.0f,  0.0f, 0.0f)), 
        Vertex(vec3(0.5f, 0.5f, -0.5f),  vec3(1.0f,  0.0f, 0.0f)),
        Vertex(vec3(0.5f, -0.5f, -0.5f),  vec3(1.0f,  0.0f, 0.0f)),
        Vertex(vec3(0.5f, -0.5f, -0.5f),  vec3(1.0f,  0.0f, 0.0f)),
        Vertex(vec3(0.5f, -0.5f, 0.5f),  vec3(1.0f,  0.0f, 0.0f)),
        Vertex(vec3(0.5f, 0.5f, 0.5f),  vec3(1.0f,  0.0f, 0.0f)), 

        Vertex(vec3(-0.5f, -0.5f, -0.5f),  vec3(0.0f,  -1.0f, 0.0f)), 
        Vertex(vec3(0.5f, -0.5f, -0.5f),  vec3(0.0f,  -1.0f, 0.0f)),
        Vertex(vec3(0.5f, -0.5f, 0.5f),  vec3(0.0f,  -1.0f, 0.0f)),
        Vertex(vec3(0.5f, -0.5f, 0.5f),  vec3(0.0f,  -1.0f, 0.0f)),
        Vertex(vec3(-0.5f, -0.5f, 0.5f),  vec3(0.0f,  -1.0f, 0.0f)),
        Vertex(vec3(-0.5f, -0.5f, -0.5f),  vec3(0.0f,  -1.0f, 0.0f)),

        Vertex(vec3(-0.5f, 0.5f, -0.5f),  vec3(0.0f,  1.0f, 0.0f)), 
        Vertex(vec3(0.5f, 0.5f, -0.5f),  vec3(0.0f,  1.0f, 0.0f)),
        Vertex(vec3(0.5f, 0.5f, 0.5f),  vec3(0.0f,  1.0f, 0.0f)),
        Vertex(vec3(0.5f, 0.5f, 0.5f),  vec3(0.0f,  1.0f, 0.0f)),
        Vertex(vec3(-0.5f, 0.5f, 0.5f),  vec3(0.0f,  1.0f, 0.0f)),
        Vertex(vec3(-0.5f, 0.5f, -0.5f),  vec3(0.0f,  1.0f, 0.0f)),
    };

    indices = {
        0,1,2,
        3,4,5,
        6,7,8,
        9,10,11,
        12,13,14,
        15,16,17,
        18,19,20,
        21,22,23,
        24,25,26,
        27,28,29,
        30,31,32,
        33,34,35,
    };

}

void build_trapezoid(vector<Vertex>& vertices, vector<int>& indices){
    vertices = {
        Vertex(vec3(-0.5f, 0.5f, 0.5f),  vec3(0.0f,  1.0f, 0.0f)), 
        Vertex(vec3(-0.5f, 0.5f, -0.5f),  vec3(0.0f,  1.0f, 0.0f)),
        Vertex(vec3(0.5f, 0.5f, -0.5f),  vec3(0.0f,  1.0f, 0.0f)),
        Vertex(vec3(-0.5f, 0.5f, 0.5f),  vec3(0.0f,  1.0f, 0.0f)), 
        Vertex(vec3(0.5f, 0.5f, -0.5f),  vec3(0.0f,  1.0f, 0.0f)),
        Vertex(vec3(0.5f, 0.5f, 0.5f),  vec3(0.0f,  1.0f, 0.0f)),

        Vertex(vec3(0.8f, -0.5f, 0.8f),  vec3(1.0f,  0.3f, 0.0f)), 
        Vertex(vec3(0.5f, 0.5f, 0.5f),  vec3(1.0f,  0.3f, 0.0f)),
        Vertex(vec3(0.5f, 0.5f, -0.5f),  vec3(1.0f,  0.3f, 0.0f)),
        Vertex(vec3(0.8f, -0.5f, 0.8f),  vec3(1.0f,  0.3f, 0.0f)),
        Vertex(vec3(0.5f, 0.5f, -0.5f),  vec3(1.0f,  0.3f, 0.0f)),
        Vertex(vec3(0.8f, -0.5f, -0.8f),  vec3(1.0f,  0.3f, 0.0f)), 

        Vertex(vec3(-0.8f, -0.5f, 0.8f),  vec3(0.0f,  0.3f, 1.0f)), 
        Vertex(vec3(-0.5f, 0.5f, 0.5f),  vec3(0.0f,  0.3f, 1.0f)),
        Vertex(vec3(0.5f, 0.5f, 0.5f),  vec3(0.0f,  0.3f, 1.0f)),
        Vertex(vec3(-0.8f, -0.5f, 0.8f),  vec3(0.0f,  0.3f, 1.0f)), 
        Vertex(vec3(0.5f, 0.5f, 0.5f),  vec3(0.0f,  0.3f, 1.0f)),
        Vertex(vec3(0.8f, -0.5f, 0.8f),  vec3(0.0f,  0.3f, 1.0f)),
       
        Vertex(vec3(-0.8f, -0.5f, 0.8f),  vec3(-1.0f,  0.3f, 0.0f)), 
        Vertex(vec3(-0.5f, 0.5f, 0.5f),  vec3(-1.0f,  0.3f, 0.0f)),
        Vertex(vec3(-0.5f, 0.5f, -0.5f),  vec3(-1.0f,  0.3f, 0.0f)),
        Vertex(vec3(-0.8f, -0.5f, 0.8f),  vec3(-1.0f,  0.3f, 0.0f)),
        Vertex(vec3(-0.5f, 0.5f, -0.5f),  vec3(-1.0f,  0.3f, 0.0f)),
        Vertex(vec3(-0.8f, -0.5f, -0.8f),  vec3(-1.0f,  0.3f, 0.0f)), 

        Vertex(vec3(-0.8f, -0.5f, -0.8f),  vec3(0.0f,  0.3f, -1.0f)), 
        Vertex(vec3(-0.5f, 0.5f, -0.5f),  vec3(0.0f,  0.3f, -1.0f)),
        Vertex(vec3(0.5f, 0.5f, -0.5f),  vec3(0.0f,  0.3f, -1.0f)),
        Vertex(vec3(-0.8f, -0.5f, -0.8f),  vec3(0.0f,  0.3f, -1.0f)), 
        Vertex(vec3(0.5f, 0.5f, -0.5f),  vec3(0.0f,  0.3f, -1.0f)),
        Vertex(vec3(0.8f, -0.5f, -0.8f),  vec3(0.0f,  0.3f, -1.0f)),

        Vertex(vec3(-0.8f, -0.5f, 0.8f),  vec3(0.0f,  -1.0f, 0.0f)), 
        Vertex(vec3(-0.8f, -0.5f, -0.8f),  vec3(0.0f,  -1.0f, 0.0f)),
        Vertex(vec3(0.8f, -0.5f, -0.8f),  vec3(0.0f,  -1.0f, 0.0f)),
        Vertex(vec3(-0.8f, -0.5f, 0.8f),  vec3(0.0f,  -1.0f, 0.0f)), 
        Vertex(vec3(0.8f, -0.5f, -0.8f),  vec3(0.0f,  -1.0f, 0.0f)),
        Vertex(vec3(0.8f, -0.5f, 0.8f),  vec3(0.0f,  -1.0f, 0.0f)),
    };

    indices = {
        0,1,2,
        3,4,5,
        6,7,8,
        9,10,11,
        12,13,14,
        15,16,17,
        18,19,20,
        21,22,23,
        24,25,26,
        27,28,29,
        30,31,32,
        33,34,35,
    };
}

void build_cylinder(vector<Vertex>& vertices, vector<int>& indices){
    int n=360;

    for (int i=0;i<n;i++){
        double theta= (2*(M_PI)*i)/n;
        double theta_nxt= (2*(M_PI)*(i+1))/n;
        vertices.push_back(Vertex(vec3(1.0f, 0.0f, 0.0),  vec3(1.0f, 0.0f, 0.0f)));
        vertices.push_back(Vertex(vec3(1.0f, -0.5*sin(theta), 0.5*cos(theta)),  vec3(1.0f, 0.0f, 0.0f)));
        vertices.push_back(Vertex(vec3(1.0f, -0.5*sin(theta_nxt), 0.5*cos(theta_nxt)),  vec3(1.0f, 0.0f, 0.0f)));

        vertices.push_back(Vertex(vec3(-1.0f, 0.0f, 0.0),  vec3(1.0f, 0.0f, 0.0f)));
        vertices.push_back(Vertex(vec3(-1.0f, -0.5*sin(theta), 0.5*cos(theta)),  vec3(1.0f, 0.0f, 0.0f)));
        vertices.push_back(Vertex(vec3(-1.0f, -0.5*sin(theta_nxt), 0.5*cos(theta_nxt)),  vec3(1.0f, 0.0f, 0.0f)));

        vec3 normal= vec3(0, 0.5*(cos(theta_nxt)-cos(theta)), 0.5*(sin(theta_nxt)-sin(theta)));
        vertices.push_back(Vertex(vec3(-1.0f, -0.5*sin(theta), 0.5*cos(theta)),  normal));
        vertices.push_back(Vertex(vec3(1.0f, -0.5*sin(theta), 0.5*cos(theta)),  normal));
        vertices.push_back(Vertex(vec3(1.0f, -0.5*sin(theta_nxt), 0.5*cos(theta_nxt)),  normal));
        
        vertices.push_back(Vertex(vec3(-1.0f, -0.5*sin(theta), 0.5*cos(theta)),  normal));
        vertices.push_back(Vertex(vec3(1.0f, -0.5*sin(theta_nxt), 0.5*cos(theta_nxt)),  normal));
        vertices.push_back(Vertex(vec3(-1.0f, -0.5*sin(theta_nxt), 0.5*cos(theta_nxt)),  normal));
    }

    for (int i=0;i<12*n;i++){
        indices.push_back(i);
    }
}

void build_torus(vector<Vertex>& vertices, vector<int>& indices){
    vector<Vertex> temp;
    int n=360;
    for (int i=0;i<n;i++){
        double theta= (2*(M_PI)*i)/n;
        double theta_nxt= (2*(M_PI)*(i+1))/n;
        double x_transform=(1.0f/30.0f);
        double radius=0.2;
        double z_transform=1;
        temp.push_back(Vertex(vec3(x_transform, 0.0f, z_transform+0.0f),  vec3(1.0f, 0.0f, 0.0f)));
        temp.push_back(Vertex(vec3(x_transform, -radius*sin(theta), z_transform+radius*cos(theta)),  vec3(1.0f, 0.0f, 0.0f)));
        temp.push_back(Vertex(vec3(x_transform, -radius*sin(theta_nxt), z_transform+radius*cos(theta_nxt)),  vec3(1.0f, 0.0f, 0.0f)));

        temp.push_back(Vertex(vec3(-x_transform, 0.0f, z_transform+0.0f),  vec3(1.0f, 0.0f, 0.0f)));
        temp.push_back(Vertex(vec3(-x_transform, -radius*sin(theta), z_transform+radius*cos(theta)),  vec3(1.0f, 0.0f, 0.0f)));
        temp.push_back(Vertex(vec3(-x_transform, -radius*sin(theta_nxt), z_transform+radius*cos(theta_nxt)),  vec3(1.0f, 0.0f, 0.0f)));

        vec3 normal= vec3(0, radius*(cos(theta_nxt)-cos(theta)), radius*(sin(theta_nxt)-sin(theta)));
        temp.push_back(Vertex(vec3(-x_transform, -radius*sin(theta), z_transform+radius*cos(theta)),  normal));
        temp.push_back(Vertex(vec3(x_transform, -radius*sin(theta), z_transform+radius*cos(theta)),  normal));
        temp.push_back(Vertex(vec3(x_transform, -radius*sin(theta_nxt), z_transform+radius*cos(theta_nxt)),  normal));
        
        temp.push_back(Vertex(vec3(-x_transform, -radius*sin(theta), z_transform+radius*cos(theta)),  normal));
        temp.push_back(Vertex(vec3(x_transform, -radius*sin(theta_nxt), z_transform+radius*cos(theta_nxt)),  normal));
        temp.push_back(Vertex(vec3(-x_transform, -radius*sin(theta_nxt), z_transform+radius*cos(theta_nxt)),  normal));
    }

    mat3 y_trans;
    for (int i=0;i<n;i++){
        double theta= (2*(M_PI)*i)/n;
        y_trans=mat3(cos(theta),0,sin(theta),0,1,0,-sin(theta),0,cos(theta));
        for (auto x:temp){
            Vertex final=x;
            final.Position=y_trans*final.Position;
            final.Normal=y_trans*final.Normal;
            vertices.push_back(final);
        }
    }

    for (int i=0;i<360*12*n;i++){
        indices.push_back(i);
    }
}

void build_fanplate(vector<Vertex>& vertices, vector<int>& indices){
    vector<Vertex> temp={
        Vertex(vec3(0.0f, 0.0f, 0.0f),  vec3(0.0f,  0.0f, 1.0f)), 
        Vertex(vec3(1.0f, 0.1f, 0.0f),  vec3(0.0f,  0.0f, 1.0f)),
        Vertex(vec3(1.0f, 0.0f, 0.0f),  vec3(0.0f,  0.0f, 1.0f)),

        Vertex(vec3(0.0f, 0.0f, 0.0f),  vec3(-0.04f,  0.4f, -0.1f)), 
        Vertex(vec3(1.0f, 0.1f, 0.0f),  vec3(-0.04f,  0.4f, -0.1f)),
        Vertex(vec3(1.0f, 0.0f, -0.4f),  vec3(-0.04f,  0.4f, -0.1f)),

        Vertex(vec3(0.0f, 0.0f, 0.0f),  vec3(0.0f,  -1.0f, 0.0f)), 
        Vertex(vec3(1.0f, 0.0f, 0.0f),  vec3(0.0f,  -1.0f, 0.0f)),
        Vertex(vec3(1.0f, 0.0f, -0.4f),  vec3(0.0f,  -1.0f, 0.0f)),

        Vertex(vec3(1.0f, 0.1f, 0.0f),  vec3(1.0f,  0.0f, 0.0f)),
        Vertex(vec3(1.0f, 0.0f, 0.0f),  vec3(1.0f,  0.0f, 0.0f)),
        Vertex(vec3(1.0f, 0.0f, -0.4f),  vec3(1.0f,  0.0f, 0.0f)),
    };

    vertices=temp;
    mat3 y_trans;
    double theta= (2*(M_PI))/3;
    y_trans=mat3(cos(theta),0,sin(theta),0,1,0,-sin(theta),0,cos(theta));
    for (auto x:temp){
        Vertex final=x;
        final.Position=y_trans*final.Position;
        final.Normal=y_trans*final.Normal;
        vertices.push_back(final);
    }
    theta= (4*(M_PI))/3;
    y_trans=mat3(cos(theta),0,sin(theta),0,1,0,-sin(theta),0,cos(theta));
    for (auto x:temp){
        Vertex final=x;
        final.Position=y_trans*final.Position;
        final.Normal=y_trans*final.Normal;
        vertices.push_back(final);
    }

    indices = {
        0,1,2,
        3,4,5,
        6,7,8,
        9,10,11,
        12,13,14,
        15,16,17,
        18,19,20,
        21,22,23,
        24,25,26,
        27,28,29,
        30,31,32,
        33,34,35,
    };
}

#endif 