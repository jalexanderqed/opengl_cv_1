#define GLEW_STATIC

#include <GL/glew.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <sstream>
#include <cmath>
#include <zconf.h>

#include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtc/type_ptr.hpp"
#include "lib/glm/gtx/polar_coordinates.hpp"

#include "shapes/Box.h"
#include "util/DrawState.h"
#include "util/Model.h"
#include "shapes/ImageSquare.h"

using namespace std;
using namespace cv;

DrawState drawState;

Box box;

Model nanosuit;

int screenWidth, screenHeight;

Mat image;
GLuint imageTexture;
ImageSquare imageSquare;
VideoCapture capture;

void setupNextImage();

void draw() {
    setupNextImage();
    drawState.useShader(IMAGE_PROGRAM);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, imageTexture);
    glUniform1i(glGetUniformLocation(drawState.getShader(IMAGE_PROGRAM).ProgramId, "imageTexture"), 0);

    drawState.setLocation(0, 0, 1);
    drawState.lookAt(glm::vec3(0, 0, 0));

    imageSquare.draw();

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

string type2str(int type) {
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch ( depth ) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default:     r = "User"; break;
    }

    r += "C";
    r += (chans+'0');

    return r;
}

void setupNextImage(){
    capture >> image;
    glGenTextures(1, &imageTexture);
    glBindTexture(GL_TEXTURE_2D, imageTexture);

    // Set texture interpolation methods for minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set texture clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    Size s = image.size();
    cv::flip(image, image, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, s.width, s.height, 0, GL_BGR, GL_UNSIGNED_BYTE, image.ptr());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void initObjects() {
    box = Box(1.0f, 1.0f, 1.0f);
    nanosuit = Model("objects/nanosuit/nanosuit.obj");

    imageSquare = ImageSquare(true);
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

    if ( argc == 1 ) {
        // start video capture from camera
        capture = VideoCapture(0);
    } else if ( argc == 2 ) {
        // start video capture from file
        capture = VideoCapture(argv[1]);
    } else {
        fprintf( stderr, "usage: %s [<filename>]\n", argv[0] );
        return 1;
    }

    // check that video is opened
    if ( !capture.isOpened() ) {
        fprintf( stderr, "could not start video capture\n" );
        return 1;
    }

    capture >> image;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(image.size().width, image.size().height, "glhw_01", nullptr, nullptr);
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
    glCullFace(GL_BACK);

    int lastSecond = 0;
    int frames = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClearDepth(1);
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
