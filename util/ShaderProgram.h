//
// Created by john on 10/12/16.
//

#ifndef GLHW_01_SHADER_H
#define GLHW_01_SHADER_H

#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <string>
#include "util.h"

class ShaderProgram {
public:
    GLuint ProgramId;

    ShaderProgram() {}

    ShaderProgram(const GLchar *vertexPath, const GLchar *fragmentPath);

    void use();
};

#endif //GLHW_01_SHADER_H
