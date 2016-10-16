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
#include "lib/glm/gtx/polar_coordinates.hpp"

#include "shapes/Box.h"
#include "util/DrawState.h"

using namespace std;

DrawState drawState;

Box box;

int screenWidth, screenHeight;

void draw() {
    drawState.useShader(BASIC_PROGRAM);

    GLfloat r = 10.0;
    GLfloat xPos = r * cos(glfwGetTime());
    GLfloat yPos = 2.0;
    GLfloat zPos = r * sin(glfwGetTime());

    /*
    GLfloat xPos = 2.0;
    GLfloat yPos = -2.0;
    GLfloat zPos = 5.0;
     */

    drawState.setLocation(xPos, yPos, zPos);
    drawState.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    drawState.setModelMat(model);
    box.draw();

    model = glm::translate(model, glm::vec3(2.0f, 0.0f, -2.0f));
    model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    drawState.setModelMat(model);
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

void windowSizeCallback(GLFWwindow *window, int w, int h) {
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    drawState.setScreenDims(w, h);
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

    initObjects();
    drawState.init();

    glm::vec3 initialLocation(2.0f, 2.0f, 5.0f);

    drawState.setLocation(initialLocation);
    drawState.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    windowSizeCallback(window, screenWidth, screenHeight);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    int lastSecond = 0;
    int frames = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw();
        glfwSwapBuffers(window);

        int time = (int) glfwGetTime();
        if (time > lastSecond) {
            cout << "FPS: " << frames << endl;
            lastSecond = time;
            frames = 0;
        }
        frames++;
    }

    glfwTerminate();

    return 0;
}