//
// Created by john on 10/14/16.
//

#include "Box.h"

Box::Box(GLfloat w, GLfloat l, GLfloat h){
    width = w;
    length = l;
    height = h;

    GLfloat hWidth = width / 2;
    GLfloat hLength = length / 2;
    GLfloat hHeight = height / 2;

    GLfloat vertices[] = {
            hWidth, hHeight, hLength, 1.0f, 0.0f, 0.0f,
            -hWidth, hHeight, hLength, 1.0f, 0.0f, 0.0f,
            -hWidth, -hHeight, hLength, 1.0f, 0.0f, 0.0f,
            hWidth, -hHeight, hLength, 1.0f, 0.0f, 0.0f,
            hWidth, hHeight, -hLength, 1.0f, 0.0f, 0.0f,
            -hWidth, hHeight, -hLength, 1.0f, 0.0f, 0.0f,
            -hWidth, -hHeight, -hLength, 1.0f, 0.0f, 0.0f,
            hWidth, -hHeight, -hLength, 1.0f, 0.0f, 0.0f
    };
    GLuint indices[] = {
            // back wall
            0, 1, 2,
            0, 2, 3,
            // top
            0, 1, 5,
            0, 5, 4,
            // bottom
            3, 2, 6,
            3, 6, 7,
            // front wall
            5, 4, 6,
            4, 6, 7,
            // left side
            4, 0, 7,
            7, 0, 3,
            //right side
            1, 5, 2,
            2, 5, 6
    };

    std::cout << "Assigned arrays" << std::endl;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    std::cout << "Bound VAO" << std::endl;

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::cout << sizeof(vertices) << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Box::draw(){
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);
}

