//
// Created by john on 10/12/16.
//

#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const GLchar *vertexPath, const GLchar *fragmentPath) {
    string vertexCode = myReadFile(vertexPath);
    string fragmentCode = myReadFile(fragmentPath);

    GLint success;
    GLchar infoLog[512];
    const char *cstr;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    cstr = vertexCode.c_str();
    glShaderSource(vertexShader, 1, &cstr, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED file:"
             << vertexPath << "\n" << infoLog << endl;
        cout << vertexCode << endl;
        exit(-1);
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    cstr = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &cstr, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED file:"
             << vertexPath << "\n" << infoLog << endl;
        cout << fragmentCode << endl;
        exit(-1);
    }

    this->ProgramId = glCreateProgram();
    glAttachShader(this->ProgramId, vertexShader);
    glAttachShader(this->ProgramId, fragmentShader);
    glLinkProgram(this->ProgramId);

    glGetProgramiv(this->ProgramId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->ProgramId, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED index:" << infoLog << endl;
        exit(-1);
    }
}

void ShaderProgram::use() {
    glUseProgram(this->ProgramId);
}