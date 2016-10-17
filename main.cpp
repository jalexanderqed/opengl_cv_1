#define GLEW_STATIC

#include <GL/glew.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <sstream>
#include <cmath>

#include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtc/type_ptr.hpp"
#include "lib/glm/gtx/polar_coordinates.hpp"

#include "shapes/Box.h"
#include "util/DrawState.h"
#include "util/Model.h"

using namespace std;
using namespace cv;

DrawState drawState;

Box box;

Model nanosuit;

int screenWidth, screenHeight;

void draw() {
    GLfloat r = 5.0;
    GLfloat speedMult = 1.0;
    GLfloat xPos = r * cos(glfwGetTime() * speedMult);
    GLfloat yPos = 1.0;
    GLfloat zPos = r * sin(glfwGetTime() * speedMult);

    /*
    GLfloat xPos = 15.0;
    GLfloat yPos = 10.0;
    GLfloat zPos = 15.0;
     */

    drawState.setLocation(xPos, yPos, zPos);
    drawState.lookAt(glm::vec3(0, 0, 0));

    glm::mat4 model, baseModel;
    model = glm::translate(model,
                           glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));    // It's a bit too big for our scene, so scale it down

    drawState.setModelMat(model);
    drawState.useShader(MODEL_PROGRAM);
    nanosuit.Draw(drawState.getShader(MODEL_PROGRAM));

    model = baseModel;
    model = glm::translate(model, glm::vec3(0, 0.5f, 0));
    drawState.setModelMat(model);
    drawState.useShader(BASIC_PROGRAM);
    //box.draw();
}

void initObjects() {
    box = Box(1.0f, 1.0f, 1.0f);
    nanosuit = Model("objects/nanosuit/nanosuit.obj");
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

int main(int argc, char **argv) {
    /*
    if (argc != 2) {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread(argv[1], 1);

    if (!image.data) {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", image);

    waitKey(0);

    return 0;
     */

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
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    int lastSecond = 0;
    int frames = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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