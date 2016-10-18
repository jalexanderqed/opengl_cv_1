//
// Created by john on 10/14/16.
//

#ifndef GLHW_01_BOX_H
#define GLHW_01_BOX_H


#include <GL/glew.h>
#include <iostream>

class Box {
public:
    Box(GLfloat w, GLfloat l, GLfloat h);
    Box(){ };
    void draw();

private:
    GLuint VAO;

    GLfloat width;
    GLfloat length;
    GLfloat height;
};


#endif //GLHW_01_BOX_H
