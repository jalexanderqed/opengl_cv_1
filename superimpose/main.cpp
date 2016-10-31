#define GLEW_STATIC

#include <GL/glew.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <opencv2/opencv.hpp>

#include "lib/glm/glm.hpp"
#include "lib/glm/ext.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtc/type_ptr.hpp"
#include "lib/glm/gtx/polar_coordinates.hpp"

#include "shapes/Box.h"
#include "util/DrawState.h"
#include "util/Model.h"
#include "shapes/ImageSquare.h"
#include "shapes/Sphere.h"

using namespace std;
using namespace cv;

enum Mode {
    SPHERE_MODE,
    TEAPOT_MODE,
    SUIT_MODE
} currentMode;

DrawState drawState;

Box box;
Sphere sphere;

Model nanosuit;
Model tea;

int screenWidth, screenHeight;

glm::mat4 cameraMatrix;
float fx, fy, cx, cy, fovx, fovy;
int imageWidth, imageHeight;
Mat cvCameraMatrix;
Mat distortionCoeffs;

Mat image;
GLuint imageTexture;
ImageSquare imageSquare;
VideoCapture capture;

vector<Point3f> goalSpacePoints;
Size patternSize(8, 6);
glm::mat4 extrinsicMatrix;
bool foundBoard = false;

glm::mat4 xyFlipMat(
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
);

void setupNextImage();

void setupMatrices();

void draw() {
    setupNextImage();
    drawState.useShader(IMAGE_PROGRAM);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, imageTexture);
    glUniform1i(glGetUniformLocation(drawState.getShader(IMAGE_PROGRAM).ProgramId, "imageTexture"), 0);

    drawState.setModelMat(glm::mat4());

    imageSquare.draw();
    glClear(GL_DEPTH_BUFFER_BIT); // Because of this call to clear the depth buffer, the image MUST be drawn first

    if (foundBoard) {
        if (currentMode == TEAPOT_MODE) {
            drawState.useShader(BASIC_NO_COLOR_PROGRAM);
            setupMatrices();
            glm::mat4 model = extrinsicMatrix;
            model = glm::translate(model, glm::vec3(3.5, 2.5, 0.5f));
            model = glm::rotate(model, (GLfloat)glfwGetTime(), glm::vec3(0, 0, 1));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
            model = glm::scale(model, glm::vec3(2, 2, 2));
            drawState.setModelMat(model);
            tea.Draw(drawState.getShader(BASIC_NO_COLOR_PROGRAM));
        } else if (currentMode == SPHERE_MODE) {
            drawState.useShader(BASIC_NO_COLOR_PROGRAM);
            setupMatrices();
            for (int i = 0; i < patternSize.width; i++) {
                for (int j = 0; j < patternSize.height; j++) {
                    glm::mat4 model = extrinsicMatrix;
                    model = glm::translate(model, glm::vec3(i, j, 0));
                    drawState.setModelMat(model);
                    sphere.draw();
                }
            }
        } else if (currentMode == SUIT_MODE) {
            drawState.useShader(MODEL_PROGRAM);
            setupMatrices();
            glm::mat4 model = extrinsicMatrix;
            model = glm::translate(model, glm::vec3(3.5, 2.5, 0.5f));
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 0, 1));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            drawState.setModelMat(model);
            nanosuit.Draw(drawState.getShader(MODEL_PROGRAM));
        }
    }
}

void setupMatrices() {
    glm::mat4 vMat;
    drawState.useProjectionMat(cameraMatrix);
    drawState.useViewMat(vMat);
}

void setupNextImage() {
    Mat distorted;
    capture >> distorted;

    undistort(distorted, image, cvCameraMatrix, distortionCoeffs);

    vector<Point2f> corners;
    foundBoard = findChessboardCorners(image, patternSize, corners,
                                       CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE
                                       | CALIB_CB_FAST_CHECK);
    if (foundBoard) {
        Mat rvec, tvec;
        solvePnP(goalSpacePoints, corners, cvCameraMatrix, Mat(), rvec, tvec);

        vector<cv::Point2f> projected;
        cv::projectPoints(goalSpacePoints, rvec, tvec, cvCameraMatrix, Mat(), projected);
        for (int i = 0; i < projected.size(); i++) {
            cv::Point2f pt = projected.at(i);
            cv::circle(image, pt, 5, cv::Scalar(0, 0, 255));
        }

        extrinsicMatrix = glm::mat4();
        extrinsicMatrix = glm::scale(glm::vec3(-1, -1, -1));
        extrinsicMatrix = glm::translate(extrinsicMatrix, glm::vec3(tvec.at<double>(0, 0), tvec.at<double>(1, 0),
                                                                    tvec.at<double>(2, 0)));
        extrinsicMatrix = glm::rotate(extrinsicMatrix, (GLfloat) norm(rvec),
                                      glm::vec3(rvec.at<double>(0, 0), rvec.at<double>(0, 1), rvec.at<double>(0, 2)));
        extrinsicMatrix = extrinsicMatrix * xyFlipMat;

        glm::vec4 point(0, 0, 0, 1);
        glm::vec4 loc = cameraMatrix * extrinsicMatrix * point;
    }

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
    sphere = Sphere(0.2f);
    nanosuit = Model("objects/nanosuit/nanosuit.obj");

    imageSquare = ImageSquare(true);
    tea = Model("objects/wt_teapot.obj");

    for (int i = 0; i < patternSize.height; i++) {
        for (int j = 0; j < patternSize.width; j++) {
            goalSpacePoints.push_back(Point3f(i, j, 0));
        }
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        switch (currentMode) {
            case SPHERE_MODE:
                currentMode = TEAPOT_MODE;
                break;
            case TEAPOT_MODE:
                currentMode = SUIT_MODE;
                break;
            case SUIT_MODE:
                currentMode = SPHERE_MODE;
                break;
            default:
                currentMode = TEAPOT_MODE;
                break;
        }
    }
}

void windowSizeCallback(GLFWwindow *window, int w, int h) {
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    drawState.setScreenDims(w, h);
}

void readCameraParams() {
    float focalLength, k1, k2, p1, p2, k3;

    cin >> fovx;
    cin >> fovy;
    cin >> focalLength;
    cin >> cx;
    cin.get();
    cin >> cy;
    cin >> fx;
    cin >> fy;
    cin >> k1;
    cin >> k2;
    cin >> p1;
    cin >> p2;
    cin >> k3;

    cvCameraMatrix = Mat(3, 3, CV_64FC1);
    cvCameraMatrix.at<double>(0, 0) = fx;
    cvCameraMatrix.at<double>(0, 1) = 0;
    cvCameraMatrix.at<double>(0, 2) = cx;
    cvCameraMatrix.at<double>(1, 0) = 0;
    cvCameraMatrix.at<double>(1, 1) = fy;
    cvCameraMatrix.at<double>(1, 2) = cy;
    cvCameraMatrix.at<double>(2, 0) = 0;
    cvCameraMatrix.at<double>(2, 1) = 0;
    cvCameraMatrix.at<double>(2, 2) = 1;

    distortionCoeffs = Mat(1, 5, CV_64FC1);
    distortionCoeffs.at<double>(0, 0) = k1;
    distortionCoeffs.at<double>(0, 1) = k2;
    distortionCoeffs.at<double>(0, 2) = p1;
    distortionCoeffs.at<double>(0, 3) = p2;
    distortionCoeffs.at<double>(0, 4) = k3;

    imageWidth = image.size().width;
    imageHeight = image.size().height;

    glm::mat4 intMatrix(
            fx, 0, 0, 0,
            0, fy, 0, 0,
            cx, cy, 1, 0,
            0, 0, 0, 1
    );

    glm::mat4 convertMatrix(
            -2.0f / imageWidth, 0, 0, 0,
            0, 2.0f / imageHeight, 0, 0,
            1, -1, 0.95, -1,
            0, 0, -0.05, 0
    );

    cameraMatrix = convertMatrix * intMatrix;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        // start video capture from camera
        capture = VideoCapture(0);
    } else if (argc == 2) {
        // start video capture from file
        capture = VideoCapture(argv[1]);
    } else {
        fprintf(stderr, "usage: %s [<filename>]\n", argv[0]);
        return 1;
    }

    // check that video is opened
    if (!capture.isOpened()) {
        fprintf(stderr, "could not start video capture\n");
        return 1;
    }

    capture >> image;
    readCameraParams();

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

    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    windowSizeCallback(window, screenWidth, screenHeight);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    int lastSecond = 0;
    int frames = 0;
    currentMode = SPHERE_MODE;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw();
        glfwSwapBuffers(window);

        int time = (int) glfwGetTime();
        if (time > lastSecond) {
            lastSecond = time;
            frames = 0;
        }
        frames++;
    }

    glfwTerminate();

    return 0;
}
