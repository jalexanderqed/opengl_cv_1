//
// Created by john on 11/04/16.
//

#ifndef GLHW_01_TORUS_H
#define GLHW_01_SPHERE_H


#include <GL/glew.h>
#include <vector>
#include <math.h>

class Torus {
public:
    Torus(GLfloat r1, GLfloat r2);
    Torus(){ };
    void draw();

private:
    GLuint VAO;
    int numIndices;
};


#endif //GLHW_01_SPHERE_H
