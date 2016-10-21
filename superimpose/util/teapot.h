//
// Created by john on 10/21/16.
//

#ifndef GLHW_01_TEAPOT_H
#define GLHW_01_TEAPOT_H

#include <GL/glew.h>
#include "../objects/teapot_obj.h"

class teapot {
public:
    teapot(){}
    teapot(bool placeholder);
    void draw();

private:
    GLuint VAO, VBO, NBO, EBO;
};


#endif //GLHW_01_TEAPOT_H
