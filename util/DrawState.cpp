//
// Created by john on 10/15/16.
//

#include "DrawState.h"

void DrawState::setUniforms() {
    updateModelMatrix();
    updateViewMatrix();
    updateProjectionMatrix();
    updateLightsAndColors();
}

void DrawState::setModelMat(glm::mat4 mat) {
    modelMatrix = mat;
    normalModelMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
    updateModelMatrix();
}

void DrawState::setDirection(GLfloat p, GLfloat y) {
    if (p > 89.0f) {
        p = 89.0f;
    } else if (p < -89.0f) {
        p = -89.0f;
    }
    camera.pitch = p;
    camera.yaw = y;
    camera.lookAtPoint = false;
    updateViewMatrix();
}

void DrawState::lookAt(GLfloat x, GLfloat y, GLfloat z) {
    camera.lookAt = glm::vec3(x, y, z);
    camera.lookAtPoint = true;
    updateViewMatrix();
}

void DrawState::setLocation(GLfloat x, GLfloat y, GLfloat z) {
    camera.position = glm::vec3(x, y, z);
    updateViewMatrix();
}

void DrawState::setLocation(glm::vec3 loc) {
    camera.position = loc;
    updateViewMatrix();
}

void DrawState::setScreenDims(int w, int h) {
    screenWidth = w;
    screenHeight = h;
    updateProjectionMatrix();
}

void DrawState::init() {
    initShaders();
}

void DrawState::useShader(shaderProgramsEnum shader) {
    currentShader = shader;
    shaders[currentShader].use();
    updateUniformLocations();
    setUniforms();
}

void DrawState::initShaders() {
    shaders[BASIC_PROGRAM] = ShaderProgram("shaders/vert_basic.vert", "shaders/frag_basic.frag");
    shaders[MODEL_PROGRAM] = ShaderProgram("shaders/vert_model.vert", "shaders/frag_model.frag");
    shaders[IMAGE_PROGRAM] = ShaderProgram("shaders/vert_image.vert", "shaders/frag_image.frag");
    shaders[BASIC_GLOBAL_COLOR_PROGRAM] = ShaderProgram("shaders/vert_basic_global_color.vert", "shaders/frag_basic_global_color.frag");
    shaders[BASIC_GLOBAL_COLOR_NO_LIGHT_PROGRAM] = ShaderProgram("shaders/vert_basic_global_color.vert", "shaders/frag_basic_global_color_no_light.frag");
}

void DrawState::updateUniformLocations() {
    modelUniformLocation =
            glGetUniformLocation(shaders[currentShader].ProgramId, MODEL_MAT_NAME);
    viewUniformLocation =
            glGetUniformLocation(shaders[currentShader].ProgramId, VIEW_MAT_NAME);
    projectionUniformLocation =
            glGetUniformLocation(shaders[currentShader].ProgramId, PROJECTION_MAT_NAME);
    normalModelUniformLocation =
            glGetUniformLocation(shaders[currentShader].ProgramId, NORMAL_MODEL_MAT_NAME);

    diffuseAngleUniformLocation =
            glGetUniformLocation(shaders[currentShader].ProgramId, "diffuseAngle");
    diffuseColorUniformLocation =
            glGetUniformLocation(shaders[currentShader].ProgramId, "diffuseColor");
    diffuseStrengthUniformLocation =
            glGetUniformLocation(shaders[currentShader].ProgramId, "diffuseStrength");
    ambientStrengthUniformLocation =
            glGetUniformLocation(shaders[currentShader].ProgramId, "ambientStrength");
    ambientColorUniformLocation =
            glGetUniformLocation(shaders[currentShader].ProgramId, "ambientColor");
    globalColorUniformLocation =
            glGetUniformLocation(shaders[currentShader].ProgramId, "globalColorVar");
}

void DrawState::updateViewMatrix() {
    if (camera.lookAtPoint) {
        viewMatrix = glm::lookAt(camera.position, camera.lookAt, camera.up);
    } else {
        glm::vec3 front;
        front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
        front.y = sin(glm::radians(camera.pitch));
        front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
        front = glm::normalize(front);

        glm::vec3 right = glm::normalize(glm::cross(front, camera.up));
        glm::vec3 up = glm::normalize(glm::cross(right, front));

        viewMatrix = glm::lookAt(camera.position, camera.position + front, up);
    }

    glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

void DrawState::updateProjectionMatrix() {
    projectionMatrix =
            glm::perspective(glm::radians(45.0f), ((float) screenWidth) / screenHeight, 0.05f, 500.0f);
    glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void DrawState::updateModelMatrix() {
    glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix3fv(normalModelUniformLocation, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
}

void DrawState::updateLightsAndColors() {
    glUniform3fv(diffuseAngleUniformLocation, 1, glm::value_ptr(diffuseAngle));
    glUniform3fv(diffuseColorUniformLocation, 1, glm::value_ptr(diffuseColor));
    glUniform1f(diffuseStrengthUniformLocation, diffuseStrength);
    glUniform1f(ambientStrengthUniformLocation, ambientStrength);
    glUniform3fv(ambientColorUniformLocation, 1, glm::value_ptr(ambientColor));
    glUniform3fv(globalColorUniformLocation, 1, glm::value_ptr(globalColor));
}

void DrawState::useViewMat(glm::mat4 v){
    viewMatrix = v;
    glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

void DrawState::useProjectionMat(glm::mat4 p){
    projectionMatrix = p;
    glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}