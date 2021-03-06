//
// Created by john on 10/15/16.
//

#ifndef GLHW_01_DRAWSTATE_H
#define GLHW_01_DRAWSTATE_H


#include <GL/glew.h>
#include <stack>
#include "../lib/glm/detail/type_mat.hpp"
#include "../lib/glm/detail/type_mat4x4.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/gtc/type_ptr.hpp"
#include "../util/const.h"
#include "../util/ShaderProgram.h"

struct Camera {
    // pitch and yaw are in degrees
    GLfloat pitch;
    GLfloat yaw;
    bool lookAtPoint = false;
    glm::vec3 lookAt;

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 position;
};

class DrawState {
public:
    void setUniforms();

    void setModelMat(glm::mat4 mat);

    void setDirection(GLfloat p, GLfloat y);

    void lookAt(GLfloat x, GLfloat y, GLfloat z);

    void setLocation(GLfloat x, GLfloat y, GLfloat z);

    void setLocation(glm::vec3 loc);

    void setScreenDims(int w, int h);

    void init();

    void useShader(shaderProgramsEnum shader);

    ShaderProgram getShader(shaderProgramsEnum shader) { return shaders[shader]; }

    glm::mat4 getModelMatrix() { return modelMatrix; }

    GLfloat getPitch() { return camera.pitch; }

    GLfloat getYaw() { return camera.yaw; }

    glm::vec3 getPosition() { return camera.position; }

    void setDiffuseAngle(glm::vec3 angle) {
        diffuseAngle = glm::normalize(angle);
        updateLightsAndColors();
    }

    void setDiffuseColor(glm::vec3 color) {
        diffuseColor = color;
        updateLightsAndColors();
    }

    void setDiffuseStrength(GLfloat strength) {
        diffuseStrength = strength;
        updateLightsAndColors();
    }

    void setAmbientStrength(GLfloat strength) {
        ambientStrength = strength;
        updateLightsAndColors();
    }

    void setAmbientColor(glm::vec3 color) {
        ambientColor = color;
        updateLightsAndColors();
    }

    void setGlobalColor(glm::vec3 color){
        globalColor = color;
        updateLightsAndColors();
    }

    void useViewMat(glm::mat4 v);
    void useProjectionMat(glm::mat4 p);

private:
    void initShaders();

    void updateUniformLocations();

    void updateViewMatrix();

    void updateProjectionMatrix();

    void updateModelMatrix();

    void updateLightsAndColors();

    ShaderProgram shaders[SHADER_MAX];

    shaderProgramsEnum currentShader;

    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat3 normalModelMatrix;

    Camera camera;
    int screenWidth;
    int screenHeight;

    GLuint modelUniformLocation;
    GLuint viewUniformLocation;
    GLuint projectionUniformLocation;
    GLuint normalModelUniformLocation;
    GLuint globalColorUniformLocation;

    glm::vec3 diffuseAngle = glm::normalize(glm::vec3(2, -2, -4));
    glm::vec3 diffuseColor = glm::vec3(1.0, 1.0, 1.0);
    GLfloat diffuseStrength = 0.8f;
    GLfloat ambientStrength = 0.2f;
    glm::vec3 ambientColor = glm::vec3(1.0, 1.0, 1.0);
    glm::vec3 globalColor = glm::vec3(0.7, 0.7, 0.7);

    GLuint diffuseAngleUniformLocation;
    GLuint diffuseColorUniformLocation;
    GLuint diffuseStrengthUniformLocation;
    GLuint ambientStrengthUniformLocation;
    GLuint ambientColorUniformLocation;
};


#endif //GLHW_01_DRAWSTATE_H
