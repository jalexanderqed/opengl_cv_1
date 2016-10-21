//
// Created by john on 10/21/16.
//

#include <iostream>
#include "teapot.h"

teapot::teapot(bool placeholder){
    // Create buffers/arrays
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->NBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(teapot_vertices), teapot_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->NBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(teapot_normals), teapot_normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(teapot_indices), teapot_indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void teapot::draw() {
    //glFrontFace(GL_CCW);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_STRIP, num_teapot_indices, GL_UNSIGNED_SHORT, 0);
}