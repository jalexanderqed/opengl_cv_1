#define GLEW_STATIC

#include <GL/glew.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <opencv2/opencv.hpp>

#include "lib/glm/glm.hpp"
#include "lib/glm/ext.hpp"

#include "shapes/Box.h"
#include "util/DrawState.h"
#include "util/Model.h"
#include "shapes/ImageSquare.h"
#include "shapes/Sphere.h"
#include "shapes/Cone.h"
#include "shapes/Torus.h"
#include "util/SelectBox.h"

using namespace std;
using namespace cv;

enum Mode {
    CONE_MODE = 0,
    TEAPOT_MODE,
    TORUS_MODE
} currentMode;

bool faceRectOn = false;
bool rotatingObjectsOn = false;
bool showManipulatedImage = false;
bool useSubtraction = true;

const int BACK_UPPER_X = 20;
const int BACK_UPPER_Y = 20;
const int BACK_LOWER_X = 60;
const int BACK_LOWER_Y = 60;

const int FOR_UPPER_X = 580;
const int FOR_UPPER_Y = 20;
const int FOR_LOWER_X = 620;
const int FOR_LOWER_Y = 60;

const int DEFAULT_THRESHOLD = 500;
const int DEFAULT_DELAY = 2;

SelectBox backBox(DEFAULT_THRESHOLD, DEFAULT_DELAY);
SelectBox forBox(DEFAULT_THRESHOLD, DEFAULT_DELAY);

DrawState drawState;

Box box;
Sphere sphere;
Cone cone;
Torus torus;

Model tea;

int screenWidth, screenHeight;

glm::mat4 cameraMatrix;
float fx, fy, cx, cy, fovx, fovy;
int imageWidth, imageHeight;
Mat cvCameraMatrix;
Mat distortionCoeffs;

Mat image, grayImage, smallGrayImage, savedImage, lastImage, manipulatedImage, manipulatedImageLarge;
GLuint imageTexture;
ImageSquare imageSquare;
VideoCapture capture;

glm::mat4 extrinsicMatrix;

bool foundFace = false;
int faceX, faceY, imageFaceWidth, imageFaceHeight;
const float faceWidthMM = 200;
float faceHeightMM = 200;
Point2f faceLocation;
float faceDist;

CascadeClassifier classifier("/usr/share/opencv/haarcascades/haarcascade_frontalface_default.xml");

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


    if (foundFace) {
        glm::mat4 centeredModel, model;
        centeredModel = glm::translate(extrinsicMatrix, glm::vec3(-1 * faceLocation.x, -1 * faceLocation.y, 0));

        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        drawState.useShader(BASIC_GLOBAL_COLOR_NO_LIGHT_PROGRAM);
        setupMatrices();
        drawState.setGlobalColor(glm::vec3(1, 1, 1));
        model = glm::scale(centeredModel, glm::vec3(faceWidthMM, faceHeightMM, 0.01f));
        drawState.setModelMat(model);
        box.draw();
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        if (rotatingObjectsOn) {
            if (currentMode == TEAPOT_MODE) {
                drawState.useShader(BASIC_GLOBAL_COLOR_PROGRAM);
                setupMatrices();
                drawState.setGlobalColor(glm::vec3(1, 1, 1));
                model = centeredModel;

                float r = 1.5f * faceWidthMM;
                float theta = glfwGetTime() * -2;
                model = glm::rotate(model, theta, glm::vec3(0, 1, 0));
                model = glm::translate(model, glm::vec3(r, 0, 0));
                model = glm::scale(model, glm::vec3(100, 100, 100));
                drawState.setModelMat(model);
                tea.Draw(drawState.getShader(BASIC_GLOBAL_COLOR_PROGRAM));
            } else if (currentMode == CONE_MODE) {
                drawState.useShader(BASIC_GLOBAL_COLOR_PROGRAM);
                setupMatrices();
                drawState.setGlobalColor(glm::vec3(1, 1, 1));
                model = centeredModel;

                float r = 1.5f * faceWidthMM;
                float theta = glfwGetTime() * -2;
                model = glm::rotate(model, theta, glm::vec3(0, 1, 0));
                model = glm::translate(model, glm::vec3(r, 0, 0));
                //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
                model = glm::scale(model, glm::vec3(50, 50, 50));
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
                drawState.setModelMat(model);
                cone.draw();
            } else if (currentMode == TORUS_MODE) {
                drawState.useShader(BASIC_GLOBAL_COLOR_PROGRAM);
                setupMatrices();
                drawState.setGlobalColor(glm::vec3(1, 1, 1));
                model = centeredModel;

                float r = 1.5f * faceWidthMM;
                float theta = glfwGetTime() * -2;
                model = glm::rotate(model, theta, glm::vec3(0, 1, 0));
                model = glm::translate(model, glm::vec3(r, 0, 0));
                model = glm::scale(model, glm::vec3(30, 30, 30));
                model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1, 0, 0));
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
                drawState.setModelMat(model);
                torus.draw();
            }
        }

        if (faceRectOn) {
            drawState.useShader(BASIC_GLOBAL_COLOR_NO_LIGHT_PROGRAM);
            setupMatrices();
            drawState.setGlobalColor(glm::vec3(1, 0, 0));

            model = glm::translate(centeredModel, glm::vec3(faceWidthMM / 2, faceHeightMM / 2, 0));
            model = glm::scale(model, glm::vec3(5, 5, 5));
            drawState.setModelMat(model);
            sphere.draw();

            model = glm::translate(centeredModel, glm::vec3(-1 * faceWidthMM / 2, faceHeightMM / 2, 0));
            model = glm::scale(model, glm::vec3(5, 5, 5));
            drawState.setModelMat(model);
            sphere.draw();

            model = glm::translate(centeredModel, glm::vec3(-1 * faceWidthMM / 2, -1 * faceHeightMM / 2, 0));
            model = glm::scale(model, glm::vec3(5, 5, 5));
            drawState.setModelMat(model);
            sphere.draw();

            model = glm::translate(centeredModel, glm::vec3(faceWidthMM / 2, -1 * faceHeightMM / 2, 0));
            model = glm::scale(model, glm::vec3(5, 5, 5));
            drawState.setModelMat(model);
            sphere.draw();
        }
    }
}

void setupMatrices() {
    glm::mat4 vMat;
    drawState.useProjectionMat(cameraMatrix);
    drawState.useViewMat(vMat);
}

string type2str(int type) {
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch (depth) {
        case CV_8U:
            r = "8U";
            break;
        case CV_8S:
            r = "8S";
            break;
        case CV_16U:
            r = "16U";
            break;
        case CV_16S:
            r = "16S";
            break;
        case CV_32S:
            r = "32S";
            break;
        case CV_32F:
            r = "32F";
            break;
        case CV_64F:
            r = "64F";
            break;
        default:
            r = "User";
            break;
    }

    r += "C";
    r += (chans + '0');

    return r;
}

void setupNextImage() {
    Mat distorted, drawImage;
    capture >> distorted;
    undistort(distorted, image, cvCameraMatrix, distortionCoeffs);

    flip(image, image, 1);

    lastImage = smallGrayImage.clone();

    cvtColor(image, grayImage, CV_BGR2GRAY);
    Size newSize(320, (int) ((320.0f / image.size().width) * image.size().height));
    resize(grayImage, smallGrayImage, newSize);

    if (savedImage.empty()) {
        savedImage = smallGrayImage.clone();
    }
    if (!lastImage.empty()) {
        if (useSubtraction) {
            Size s = smallGrayImage.size();
            manipulatedImage = Mat(s, CV_8UC1);
            for (int i = 0; i < s.height; i++) {
                uchar *prevRow = savedImage.ptr<uchar>(i);
                uchar *currRow = smallGrayImage.ptr<uchar>(i);
                uchar *setRow = manipulatedImage.ptr<uchar>(i);
                for (int j = 0; j < s.width; j++) {
                    setRow[j] = abs(currRow[j] - prevRow[j]);
                }
            }
            threshold(manipulatedImage, manipulatedImage, 10, 255, 0);
        } else {
            Mat flow;
            calcOpticalFlowFarneback(lastImage, smallGrayImage, flow, 0.5, 1, 3, 1, 5, 1.1, 0);
            Size s = smallGrayImage.size();
            for (int i = 0; i < s.height; i++) {
                Vec2f *flowRow = flow.ptr<Vec2f>(i);
                uchar *setRow = manipulatedImage.ptr<uchar>(i);
                for (int j = 0; j < s.width; j++) {
                    float x = flowRow[j][0];
                    float y = flowRow[j][1];
                    setRow[j] = sqrt(x * x + y * y);
                }
            }
            threshold(manipulatedImage, manipulatedImage, 5, 255, 0);
        }
        resize(manipulatedImage, manipulatedImageLarge, image.size());
    } else {
        manipulatedImageLarge = Mat(image.size(), CV_8UC1);
    }

    if (showManipulatedImage) {
        cvtColor(manipulatedImageLarge, drawImage, CV_GRAY2BGR);
    } else {
        drawImage = image.clone();
    }

    vector<Rect> faces;
    classifier.detectMultiScale(smallGrayImage, faces, 1.1, 2, CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT);

    foundFace = faces.size() > 0;
    if (foundFace) {
        faceX = 2 * faces[0].x;
        faceY = 2 * faces[0].y;
        imageFaceWidth = 2 * faces[0].width;
        imageFaceHeight = 2 * faces[0].height;
        Point2f imageFaceCenter(faceX + imageFaceWidth / 2.0f, faceY + imageFaceHeight / 2.0f);

        if (faceRectOn) {
            rectangle(drawImage, Point2f(faceX, faceY), Point2f(faceX + imageFaceWidth, faceY + imageFaceHeight),
                      Scalar(0, 255, 255));
        }

        faceDist = fx * faceWidthMM / imageFaceWidth;
        faceHeightMM = faceDist * imageFaceHeight / fy;
        extrinsicMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, -1 * faceDist));

        faceLocation.x = (imageFaceCenter.x - cx) * (faceHeightMM / imageFaceHeight);
        faceLocation.y = (imageFaceCenter.y - cy) * (faceHeightMM / imageFaceHeight);
    }

    int backSum = sumPixels(BACK_UPPER_X, BACK_UPPER_Y, BACK_LOWER_X, BACK_LOWER_Y, manipulatedImageLarge) / 255;
    int forSum = sumPixels(FOR_UPPER_X, FOR_UPPER_Y, FOR_LOWER_X, FOR_LOWER_Y, manipulatedImageLarge) / 255;
    bool backTurnedOn = backBox.updateState(backSum);
    bool forTurnedOn = forBox.updateState(forSum);
    if (forTurnedOn) {
        currentMode = (Mode) ((currentMode + 1) % 3);
    }
    if (backTurnedOn) {
        currentMode = (Mode) (((currentMode - 1) + 3) % 3);
    }

    rectangle(drawImage,
              Point2f(BACK_UPPER_X, BACK_UPPER_Y),
              Point2f(BACK_LOWER_X, BACK_LOWER_Y),
              backBox.isClicked() ? Scalar(0, 255, 255) : Scalar(0, 0, 255),
              CV_FILLED);
    rectangle(drawImage,
              Point2f(FOR_UPPER_X, FOR_UPPER_Y),
              Point2f(FOR_LOWER_X, FOR_LOWER_Y),
              forBox.isClicked() ? Scalar(0, 255, 255) : Scalar(0, 255, 0),
              CV_FILLED);

    glBindTexture(GL_TEXTURE_2D, imageTexture);

    // Set texture interpolation methods for minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set texture clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    cv::flip(drawImage, drawImage, 0);
    Size s = drawImage.size();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, s.width, s.height, 0, GL_BGR, GL_UNSIGNED_BYTE, drawImage.ptr());

    glBindTexture(GL_TEXTURE_2D, 0);
}

void initObjects() {
    box = Box(1.0f, 1.0f, 1.0f);
    sphere = Sphere(1.0f);
    cone = Cone(1.0f, 1.0f);
    torus = Torus(1.0f, 0.2f);
    tea = Model("objects/wt_teapot.obj");

    glGenTextures(1, &imageTexture);
    imageSquare = ImageSquare(true);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        currentMode = (Mode) ((currentMode + 1) % 3);
    } else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        faceRectOn = !faceRectOn;
    } else if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        rotatingObjectsOn = !rotatingObjectsOn;
    } else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        savedImage = smallGrayImage.clone();
    } else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        showManipulatedImage = !showManipulatedImage;
    } else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        useSubtraction = !useSubtraction;
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
        capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
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
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

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
    currentMode = CONE_MODE;

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
