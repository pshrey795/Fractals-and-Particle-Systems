#include "util.hpp"

using namespace std;
using namespace glm;

#define DELTA_TIME_FLAG 20
#define DELTA_TIME_MARCH 1

int main(int argc, char** argv){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window1 = glfwCreateWindow(stoi(argv[1]),stoi(argv[2]),"Third Person View",NULL,NULL);
    if(window1==NULL){
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window1);
    glfwSetFramebufferSizeCallback(window1, framebuffer_size_callback);   

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Failed to load GLAD" << endl;
        return -1;
    }

    //Building and compiling the shaders
    Shader *shaderProgram1 = new Shader("shaders/vertex.glsl","shaders/fragment.glsl");
    time1 = 0.0f;

    vector<Vertex> vertices_body;
    vector<int> indices_body;
    build_cuboid(vertices_body,indices_body);

    vector<Vertex> vertices_earth;
    vector<int> indices_earth;
    build_cuboid(vertices_earth,indices_earth);

    Model* earth1 = new Model(0,0);
    earth1->addShape(vertices_earth,indices_earth, GROUND);
    mat4 model = mat4(1.0f);
    model = scale(model, vec3(2000.0f,1.0f,2000.0f));
    model = translate(model, vec3(0.0f, -1.0f, 0.0f));
    earth1->setTransformByID(model,0);
    
    drone1 = new Model(0,0);
    drone1->setCamera(new Camera(cameraPos1,cameraTop1,cameraTarget1));
    drone1->addShape(vertices_body,indices_body, METAL);
    model = mat4(1.0f);
    model = translate(model, vec3(10.0f, 10.0f, +10.0f));
    model = rotate(model, radians(45.0f), vec3(0.0f,1.0f,0.0f));
    drone1->setTransformByID(model,0);

    //Flag Simulation
    vector<Vertex> vertices_pole;
    vector<int> indices_pole;
    build_cylinder(vertices_pole,indices_pole);
    Model* flagPole = new Model(0,0);
    flagPole->addShape(vertices_pole,indices_pole,BROWN);
    model = mat4(1.0f);
    model = translate(model, vec3(0.0f, 5.0f, 0.0f));
    model = rotate(model, radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
    model = scale(model, vec3(5.0f,0.2f,0.2f));
    flagPole->setTransformByDepth(model,0);

    flagSystem* flag = new flagSystem(vec3(0.0f,5.0f,0.0f),vec3(1.0f,0.0f,0.0f),vec3(0.0f,1.0f,0.0f),80,50,0.1f,0.1f,0.1f);

    //March Simulation
    marchSystem* march = new marchSystem(vec3(-10.0f,1.0f,5.0f),vec3(1.0f,0.0f,0.0f),vec3(0.0f,0.0f,-1.0f),30,30,0.5f,0.5f,0.1f);


    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window1)){

        //Window 1
        glfwMakeContextCurrent(window1);
        processInput(window1);
        glClearColor(0.5f,0.7f,0.9f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // drone1->updateView(drone1->cam->getViewMatrix());
        // drone1->updatePerspective(perspective(radians(fov),(float)1600/(float)1200,0.1f,500.0f));
        // drone1->Draw(shaderProgram1,drone1->cam);

        earth1->updateView(drone1->cam->getViewMatrix());
        earth1->updatePerspective(perspective(radians(fov),(float)1600/(float)1200,0.1f,500.0f));
        earth1->Draw(shaderProgram1, drone1->cam);

        //Simulate Flag
        flagPole->updateView(drone1->cam->getViewMatrix());
        flagPole->updatePerspective(perspective(radians(fov),(float)1600/(float)1200,0.1f,500.0f));
        flagPole->Draw(shaderProgram1,drone1->cam);

        flag->updateView(drone1->cam->getViewMatrix());
        flag->updatePerspective(perspective(radians(fov),(float)1600/(float)1200,0.1f,500.0f));
        flag->update(DELTA_TIME_FLAG);
        flag->draw(shaderProgram1, drone1->cam);

        //Simulate March 
        march->updateView(drone1->cam->getViewMatrix());
        march->updatePerspective(perspective(radians(fov),(float)1600/(float)1200,0.1f,500.0f));
        march->update(DELTA_TIME_MARCH);
        march->draw(shaderProgram1, drone1->cam);

        glfwSwapBuffers(window1);

        //Events and buffer swap
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}