#define GLEW_STATIC

#include <GL/glew.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <cmath>

#include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtc/type_ptr.hpp"

#include "util/ShaderProgram.h"
#include "shapes/Box.h"

using namespace std;

ShaderProgram basicProgram;

GLuint modelUniformLocation;
GLuint viewUniformLocation;
GLuint projectionUniformLocation;

Box box;

int screenWidth, screenHeight;

void draw() {
    basicProgram.use();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),((float)screenWidth) / screenHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 view;
    GLfloat radius = 10.0f;
    //GLfloat camX = sin(glfwGetTime()) * radius;
    //GLfloat camZ = cos(glfwGetTime()) * radius;
    GLfloat camX = 3;
    GLfloat camY = 3;
    GLfloat camZ = -radius;
    view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 model;
    glm::mat4 origModel = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 toyColor(1.0f, 0.5f, 0.31f);

    glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

    box.draw();
}

void initObjects() {
    box = Box(1.0f, 1.0f, 1.0f);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void windowSizeCallback(GLFWwindow *window, int w, int h){
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
}

int createPrograms() {
    basicProgram = ShaderProgram("shaders/vert_0.vert", "shaders/frag_0.frag");

    return 0;
}

void initUniforms(){
    modelUniformLocation = glGetUniformLocation(basicProgram.ProgramId, "model");
    viewUniformLocation = glGetUniformLocation(basicProgram.ProgramId, "view");
    projectionUniformLocation = glGetUniformLocation(basicProgram.ProgramId, "projection");
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "glhw_01", nullptr, nullptr);
    if (window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }

    if (createPrograms() != 0) {
        return -1;
    }
    initObjects();
    initUniforms();

    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    glEnable(GL_DEPTH_TEST);

    int lastSecond = 0;
    int frames = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw();
        glfwSwapBuffers(window);

        int time = (int)glfwGetTime();
        if(time > lastSecond){
            cout << "FPS: " << frames << endl;
            lastSecond = time;
            frames = 0;
        }
        frames++;
    }

    glfwTerminate();

    return 0;
}