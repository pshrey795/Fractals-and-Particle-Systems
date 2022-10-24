#include "util.hpp"
#include "fractals.hpp"

using namespace std;
using namespace glm;

int main(int argc, char** argv){

    int mode = stoi(argv[3]);

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

    GLFWwindow* window2 = glfwCreateWindow(stoi(argv[1]),stoi(argv[2]),"Drone View",NULL,NULL);
    if(window2==NULL){
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window1);
    glfwSetFramebufferSizeCallback(window1, framebuffer_size_callback);   
    glfwSetFramebufferSizeCallback(window2, framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Failed to load GLAD" << endl;
        return -1;
    }

    //Building and compiling the shaders
    Shader *shaderProgram1 = new Shader("shaders/vertex.glsl","shaders/fragment.glsl");

    make_terrain_row1(0,0);

    vector<Vertex> vertices_body;
    vector<int> indices_body;
    build_cuboid(vertices_body,indices_body);
    
    vector<Vertex> vertices_arms;
    vector<int> indices_arms;
    build_cylinder(vertices_arms,indices_arms);

    vector<Vertex> vertices_fancover;
    vector<int> indices_fancover;
    build_torus(vertices_fancover,indices_fancover);
    
    vector<Vertex> vertices_fan;
    vector<int> indices_fan;
    build_fanplate(vertices_fan,indices_fan);

    vector<Vertex> vertices_front;
    vector<int> indices_front;
    build_cuboid(vertices_front,indices_front);

    vector<Vertex> vertices_earth;
    vector<int> indices_earth;
    vector<vec2> textures_earth;
    build_cuboid(vertices_earth,indices_earth);

    Model* earth1 = new Model(0,0);
    earth1->addShape(vertices_earth,indices_earth,GROUND);
    mat4 model = mat4(1.0f);
    model = scale(model, vec3(2000.0f,0.01f,2000.0f));
    earth1->setTransformByID(model,0);

    drone1 = new Model(0,0);
    drone1->setCamera(new Camera(cameraPos1,cameraTop1,cameraTarget1));
    drone1->addShape(vertices_body,indices_body,METAL);
    model = mat4(1.0f);
    model = scale(model, vec3(1.5f,0.3f,1.5f));
    drone1->setTransformByID(model,0);

    Model* front1 = new Model(13,1);
    front1->addShape(vertices_front, indices_front,PLASTIC);
    model = mat4(1.0f);
    model = scale(model, vec3(0.2f,0.2f,0.2f));
    model = translate (model, vec3(0.0f, 1.0f, -2.4f));
    front1->setTransformByID(model,13);
    drone1->addChildModel(front1);

    Model* arm1 = new Model(1,1);
    arm1->addShape(vertices_arms,indices_arms,PLASTIC);
    model = mat4(1.0f);
    model = translate(model, vec3(1.0f,0.0f,-1.0f));
    model = rotate(model, radians(45.0f), vec3(0.0f,1.0f,0.0f));
    model = scale(model,vec3(1.0f,0.2f,0.2f));
    arm1->setTransformByID(model,1);
    drone1->addChildModel(arm1);

    Model* arm2 = new Model(2,1);
    arm2->addShape(vertices_arms,indices_arms,PLASTIC);
    model = mat4(1.0f);
    model = translate(model, vec3(1.0f,0.0f,1.0f));
    model = rotate(model, radians(-45.0f), vec3(0.0f,1.0f,0.0f));
    model = scale(model,vec3(1.0f,0.2f,0.2f));
    arm2->setTransformByID(model,2);
    drone1->addChildModel(arm2);

    Model* arm3 = new Model(3,1);
    arm3->addShape(vertices_arms,indices_arms,PLASTIC);
    model = mat4(1.0f);
    model = translate(model, vec3(-1.0f,0.0f,-1.0f));
    model = rotate(model, radians(-45.0f), vec3(0.0f,1.0f,0.0f));
    model = scale(model,vec3(1.0f,0.2f,0.2f));
    arm3->setTransformByID(model,3);
    drone1->addChildModel(arm3);

    Model* arm4 = new Model(4,1);
    arm4->addShape(vertices_arms,indices_arms,PLASTIC);
    model = mat4(1.0f);
    model = translate(model, vec3(-1.0f,0.0f,1.0f));
    model = rotate(model, radians(45.0f), vec3(0.0f,1.0f,0.0f));
    model = scale(model,vec3(1.0f,0.2f,0.2f));
    arm4->setTransformByID(model,4);
    drone1->addChildModel(arm4);

    Model* fancover1 = new Model(5,2);
    fancover1->addShape(vertices_fancover,indices_fancover,RUBBER);
    model = mat4(1.0f);
    model = translate(model, vec3(2.1f,0.0f,-2.1f));
    model = scale(model,vec3(0.5f,0.5f,0.5f));
    fancover1->setTransformByID(model,5);
    arm1->addChildModel(fancover1);

    Model* fancover2 = new Model(6,2);
    fancover2->addShape(vertices_fancover,indices_fancover,RUBBER);
    model = mat4(1.0f);
    model = translate(model, vec3(2.1f,0.0f,2.1f));
    model = scale(model,vec3(0.5f,0.5f,0.5f));
    fancover2->setTransformByID(model,6);
    arm2->addChildModel(fancover2);

    Model* fancover3 = new Model(7,2);
    fancover3->addShape(vertices_fancover,indices_fancover,RUBBER);
    model = mat4(1.0f);
    model = translate(model, vec3(-2.1f,0.0f,-2.1f));
    model = scale(model,vec3(0.5f,0.5f,0.5f));
    fancover3->setTransformByID(model,7);
    arm3->addChildModel(fancover3);

    Model* fancover4 = new Model(8,2);
    fancover4->addShape(vertices_fancover,indices_fancover,RUBBER);
    model = mat4(1.0f);
    model = translate(model, vec3(-2.1f,0.0f,2.1f));
    model = scale(model,vec3(0.5f,0.5f,0.5f));
    fancover4->setTransformByID(model,8);
    arm4->addChildModel(fancover4);

    Model* fan1 = new Model(9,3);
    fan1->addShape(vertices_fan,indices_fan,BLADE);
    model = mat4(1.0f);
    model = translate(model, vec3(2.1f,0.0f,-2.1f));
    model = scale(model,vec3(0.35f,1.0f,0.35f));
    fan1->setTransformByID(model,9);
    fancover1->addChildModel(fan1);

    Model* fan2 = new Model(10,3);
    fan2->addShape(vertices_fan,indices_fan,BLADE);
    model = mat4(1.0f);
    model = translate(model, vec3(2.1f,0.0f,2.1f));
    model = scale(model,vec3(0.35f,1.0f,0.35f));
    fan2->setTransformByID(model,10);
    fancover2->addChildModel(fan2);

    Model* fan3 = new Model(11,3);
    fan3->addShape(vertices_fan,indices_fan,BLADE);
    model = mat4(1.0f);
    model = translate(model, vec3(-2.1f,0.0f,-2.1f));
    model = scale(model,vec3(0.35f,1.0f,0.35f));
    fan3->setTransformByID(model,11);
    fancover3->addChildModel(fan3);

    Model* fan4 = new Model(12,3);
    fan4->addShape(vertices_fan,indices_fan,BLADE);
    model = mat4(1.0f);
    model = translate(model, vec3(-2.1f,0.0f,2.1f));
    model = scale(model,vec3(0.35f,1.0f,0.35f));
    fan4->setTransformByID(model,12);
    fancover4->addChildModel(fan4);

    drone1->updateTransformByDepth(translate(mat4(1.0f),vec3(0.0f,10.0f,0.0f)),0);

    string s = generateTreeString(5);
    Model* tree11 = createTree(s, 20);
    model = mat4(1.0f);
    model = translate(model, vec3(40.0f,0.0f,40.0f));
    tree11->updateTransformByDepth(model,0);

    Model* tree12 = createTree(s, 20);
    model = mat4(1.0f);
    model = translate(model, vec3(40.0f,0.0f,0.0f));
    tree12->updateTransformByDepth(model,0);

    Model* tree13 = createTree(s, 20);
    model = mat4(1.0f);
    model = translate(model, vec3(0.0f,0.0f,40.0f));
    tree13->updateTransformByDepth(model,0);

    glEnable(GL_DEPTH_TEST);

    glfwMakeContextCurrent(window2);
    Shader *shaderProgram2 = new Shader("shaders/vertex.glsl","shaders/fragment.glsl");

    make_terrain_row2(0, 0);

    Model* earth2 = new Model(0,0);
    earth2->addShape(vertices_earth,indices_earth, GROUND);
    model = mat4(1.0f);
    model = scale(model, vec3(2000.0f,0.01f,2000.0f));
    model = translate(model, vec3(0.0f, 0.0f, 0.0f));
    earth2->setTransformByID(model,0);

    drone2 = new Model(0,0);
    drone2->setCamera(new Camera(cameraPos2,cameraTop2,cameraTarget2));
    drone2->setFirstPerson();
    drone2->addShape(vertices_body,indices_body,METAL);
    model=mat4(1.0f);
    model = scale(model, vec3(1.5f,0.3f,1.5f));
    drone2->setTransformByID(model,0);

    Model* front2 = new Model(13,1);
    front2->addShape(vertices_front, indices_front,PLASTIC);
    model = mat4(1.0f);
    model = scale(model, vec3(0.2f,0.2f,0.2f));
    model = translate (model, vec3(0.0f, 1.0f, -2.4f));
    front2->setTransformByID(model,13);
    drone2->addChildModel(front2);

    Model* arm11 = new Model(1,1);
    arm11->addShape(vertices_arms,indices_arms,PLASTIC);
    model = mat4(1.0f);
    model = translate(model, vec3(1.0f,0.0f,-1.0f));
    model = rotate(model, radians(45.0f), vec3(0.0f,1.0f,0.0f));
    model = scale(model,vec3(1.0f,0.2f,0.2f));
    arm11->setTransformByID(model,1);
    drone2->addChildModel(arm11);

    Model* arm12 = new Model(2,1);
    arm12->addShape(vertices_arms,indices_arms,PLASTIC);
    model = mat4(1.0f);
    model = translate(model, vec3(1.0f,0.0f,1.0f));
    model = rotate(model, radians(-45.0f), vec3(0.0f,1.0f,0.0f));
    model = scale(model,vec3(1.0f,0.2f,0.2f));
    arm12->setTransformByID(model,2);
    drone2->addChildModel(arm12);

    Model* arm13 = new Model(3,1);
    arm13->addShape(vertices_arms,indices_arms,PLASTIC);
    model = mat4(1.0f);
    model = translate(model, vec3(-1.0f,0.0f,-1.0f));
    model = rotate(model, radians(-45.0f), vec3(0.0f,1.0f,0.0f));
    model = scale(model,vec3(1.0f,0.2f,0.2f));
    arm13->setTransformByID(model,3);
    drone2->addChildModel(arm13);

    Model* arm14 = new Model(4,1);
    arm14->addShape(vertices_arms,indices_arms,PLASTIC);
    model = mat4(1.0f);
    model = translate(model, vec3(-1.0f,0.0f,1.0f));
    model = rotate(model, radians(45.0f), vec3(0.0f,1.0f,0.0f));
    model = scale(model,vec3(1.0f,0.2f,0.2f));
    arm14->setTransformByID(model,4);
    drone2->addChildModel(arm14);

    Model* fancover11 = new Model(5,2);
    fancover11->addShape(vertices_fancover,indices_fancover,RUBBER);
    model = mat4(1.0f);
    model = translate(model, vec3(2.1f,0.0f,-2.1f));
    model = scale(model,vec3(0.5f,0.5f,0.5f));
    fancover11->setTransformByID(model,5);
    arm11->addChildModel(fancover11);

    Model* fancover12 = new Model(6,2);
    fancover12->addShape(vertices_fancover,indices_fancover,RUBBER);
    model = mat4(1.0f);
    model = translate(model, vec3(2.1f,0.0f,2.1f));
    model = scale(model,vec3(0.5f,0.5f,0.5f));
    fancover12->setTransformByID(model,6);
    arm12->addChildModel(fancover12);

    Model* fancover13 = new Model(7,2);
    fancover13->addShape(vertices_fancover,indices_fancover,RUBBER);
    model = mat4(1.0f);
    model = translate(model, vec3(-2.1f,0.0f,-2.1f));
    model = scale(model,vec3(0.5f,0.5f,0.5f));
    fancover13->setTransformByID(model,7);
    arm13->addChildModel(fancover13);

    Model* fancover14 = new Model(8,2);
    fancover14->addShape(vertices_fancover,indices_fancover,RUBBER);
    model = mat4(1.0f);
    model = translate(model, vec3(-2.1f,0.0f,2.1f));
    model = scale(model,vec3(0.5f,0.5f,0.5f));
    fancover14->setTransformByID(model,8);
    arm14->addChildModel(fancover14);

    Model* fan11 = new Model(9,3);
    fan11->addShape(vertices_fan,indices_fan,BLADE);
    model = mat4(1.0f);
    model = translate(model, vec3(2.1f,0.0f,-2.1f));
    model = scale(model,vec3(0.35f,1.0f,0.35f));
    fan11->setTransformByID(model,9);
    fancover11->addChildModel(fan11);

    Model* fan12 = new Model(10,3);
    fan12->addShape(vertices_fan,indices_fan,BLADE);
    model = mat4(1.0f);
    model = translate(model, vec3(2.1f,0.0f,2.1f));
    model = scale(model,vec3(0.35f,1.0f,0.35f));
    fan12->setTransformByID(model,10);
    fancover12->addChildModel(fan12);

    Model* fan13 = new Model(11,3);
    fan13->addShape(vertices_fan,indices_fan,BLADE);
    model = mat4(1.0f);
    model = translate(model, vec3(-2.1f,0.0f,-2.1f));
    model = scale(model,vec3(0.35f,1.0f,0.35f));
    fan13->setTransformByID(model,11);
    fancover13->addChildModel(fan13);

    Model* fan14 = new Model(12,3);
    fan14->addShape(vertices_fan,indices_fan,BLADE);
    model = mat4(1.0f);
    model = translate(model, vec3(-2.1f,0.0f,2.1f));
    model = scale(model,vec3(0.35f,1.0f,0.35f));
    fan14->setTransformByID(model,12);
    fancover14->addChildModel(fan14);

    Model* tree21 = createTree(s, 20);
    model = mat4(1.0f);
    model = translate(model, vec3(40.0f,0.0f,40.0f));
    tree21->updateTransformByDepth(model,0);

    Model* tree22 = createTree(s, 20);
    model = mat4(1.0f);
    model = translate(model, vec3(40.0f,0.0f,0.0f));
    tree22->updateTransformByDepth(model,0);

    Model* tree23 = createTree(s, 20);
    model = mat4(1.0f);
    model = translate(model, vec3(0.0f,0.0f,40.0f));
    tree23->updateTransformByDepth(model,0);

    drone2->updateTransformByDepth(translate(mat4(1.0f),vec3(0.0f,10.0f,0.0f)),0);
    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window1) && !glfwWindowShouldClose(window2)){

        //Window 1
        glfwMakeContextCurrent(window1);
        if(mode){
            simulateDrone(1);
        }else{
            processInput(window1);
        }
        glClearColor(0.5f,0.7f,0.9f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drone1->updateView(drone1->cam->getViewMatrix());
        drone1->updatePerspective(perspective(radians(fov),(float)1600/(float)1200,0.1f,500.0f));
        drone1->Draw(shaderProgram1,drone1->cam);

        earth1->updateView(drone1->cam->getViewMatrix());
        earth1->updatePerspective(perspective(radians(fov),(float)1600/(float)1200,0.1f,500.0f));
        earth1->Draw(shaderProgram1, drone1->cam);

        for(int i=0;i<terrain1.size();i++){
            terrain1[i]->updateView(drone1->cam->getViewMatrix());
            terrain1[i]->updatePerspective(perspective(radians(fov),(float)1600/(float)1200,0.1f,500.0f));
            terrain1[i]->Draw(shaderProgram1, drone1->cam);
        }
        
        tree11->updateView(drone1->cam->getViewMatrix());
        tree11->updatePerspective(perspective(radians(fov),(float)1600/(float)1200,0.1f,500.0f));
        tree11->Draw(shaderProgram1, drone1->cam);

        tree12->updateView(drone1->cam->getViewMatrix());
        tree12->updatePerspective(perspective(radians(fov),(float)1600/(float)1200,0.1f,500.0f));
        tree12->Draw(shaderProgram1, drone1->cam);

        tree13->updateView(drone1->cam->getViewMatrix());
        tree13->updatePerspective(perspective(radians(fov),(float)1600/(float)1200,0.1f,500.0f));
        tree13->Draw(shaderProgram1, drone1->cam);

        glfwSwapBuffers(window1);

        //Window 2 
        glfwMakeContextCurrent(window2);
        if(mode){
            simulateDrone(2);
        }else{
            processInput(window2);
        }
        glClearColor(0.5f,0.7f,0.9f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drone2->updateView(drone2->cam->getViewMatrix());
        drone2->updatePerspective(perspective(radians(fov2),(float)1600/(float)1200,0.1f,500.0f));
        drone2->Draw(shaderProgram2,drone2->cam);

        earth2->updateView(drone2->cam->getViewMatrix());
        earth2->updatePerspective(perspective(radians(fov2),(float)1600/(float)1200,0.1f,500.0f));
        earth2->Draw(shaderProgram2, drone2->cam);

        for(int i=0;i<terrain2.size();i++){
            terrain2[i]->updateView(drone2->cam->getViewMatrix());
            terrain2[i]->updatePerspective(perspective(radians(fov),(float)1600/(float)1200,0.1f,500.0f));
            terrain2[i]->Draw(shaderProgram2, drone2->cam);
        }
    
        tree21->updateView(drone2->cam->getViewMatrix());
        tree21->updatePerspective(perspective(radians(fov2),(float)1600/(float)1200,0.1f,500.0f));
        tree21->Draw(shaderProgram2, drone2->cam);

        tree22->updateView(drone2->cam->getViewMatrix());
        tree22->updatePerspective(perspective(radians(fov2),(float)1600/(float)1200,0.1f,500.0f));
        tree22->Draw(shaderProgram2, drone2->cam);

        tree23->updateView(drone2->cam->getViewMatrix());
        tree23->updatePerspective(perspective(radians(fov2),(float)1600/(float)1200,0.1f,500.0f));
        tree23->Draw(shaderProgram2, drone2->cam);

        glfwSwapBuffers(window2);

        //Events and buffer swap
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}