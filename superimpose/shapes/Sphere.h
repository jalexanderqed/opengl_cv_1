//
// Created by john on 10/21/16.
//

#ifndef GLHW_01_SPHERE_H
#define GLHW_01_SPHERE_H


#include <GL/glew.h>
#include <vector>
#include <math.h>

class Sphere {
public:
    Sphere(GLfloat r);
    Sphere(){ };
    void draw();

private:
    GLuint VAO;
    int numIndices;
};


#endif //GLHW_01_SPHERE_H
