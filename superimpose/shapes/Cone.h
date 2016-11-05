//
// Created by john on 11/04/16.
//

#ifndef GLHW_01_CONE_H
#define GLHW_01_CONE_H


#include <GL/glew.h>
#include <vector>
#include <math.h>
#include "../lib/glm/glm.hpp"

class Cone {
public:
    Cone(GLfloat r, GLfloat h);
    Cone(){ };
    void draw();

private:
    GLuint VAO;
    int numIndices;
};


#endif //GLHW_01_SPHERE_H
