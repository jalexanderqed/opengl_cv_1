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

    GLfloat r = 20.0;
    GLfloat speedMult = 0.3;
    GLfloat xPos = r * cos(glfwGetTime() * speedMult);
    GLfloat yPos = 10.0;
    GLfloat zPos = r * sin(glfwGetTime() * speedMult);

    /*
    GLfloat xPos = 15.0;
    GLfloat yPos = 10.0;
    GLfloat zPos = 15.0;
     */

    drawState.setLocation(xPos, yPos, zPos);
    drawState.lookAt(glm::vec3(0, 3, 0));

    glm::mat4 model, baseModel;
    model = glm::scale(model, glm::vec3(3, 0.1f, 3));
    drawState.setModelMat(model);
    box.draw();

    model = baseModel = glm::translate(baseModel, glm::vec3(0, 0.1f, 0));

    model = glm::scale(model, glm::vec3(1, 3, 1));
    baseModel = glm::translate(baseModel, glm::vec3(0, 3, 0));

    model = glm::translate(model, glm::vec3(0, 0.5f, 0));
    drawState.setModelMat(model);
    box.draw();

    model = baseModel;

    GLfloat rotation = glfwGetTime();
    model = glm::rotate(model, rotation, glm::vec3(0, 1, 0));
    baseModel = glm::rotate(baseModel, rotation, glm::vec3(0, 1, 0));

    model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0, 0, 1));
    baseModel = glm::rotate(baseModel, glm::radians(35.0f), glm::vec3(0, 0, 1));

    model = glm::scale(model, glm::vec3(0.7f, 5, 0.7f));
    baseModel = glm::translate(baseModel, glm::vec3(0, 5, 0));

    model = glm::rotate(model, -1 * rotation, glm::vec3(0, 1, 0));
    baseModel = glm::rotate(baseModel, -1 * rotation, glm::vec3(0, 1, 0));

    model = glm::translate(model, glm::vec3(0, 0.5f, 0));
    drawState.setModelMat(model);
    box.draw();

    model = baseModel;

    model = glm::scale(model, glm::vec3(0.7f, 0.2f, 2.5f));
    baseModel = glm::translate(baseModel, glm::vec3(0, 0.2f, 0));

    model = glm::translate(model, glm::vec3(0, 0.5f, 0));
    drawState.setModelMat(model);
    box.draw();

    model = baseModel;

    model = glm::translate(model, glm::vec3(0, 0, -1));
    model = glm::scale(model, glm::vec3(0.7f, 1, 0.1f));
    model = glm::translate(model, glm::vec3(0, 0.5f, 0));
    drawState.setModelMat(model);
    box.draw();

    model = baseModel;

    model = glm::translate(model, glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(0.7f, 1, 0.1f));
    model = glm::translate(model, glm::vec3(0, 0.5f, 0));
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