//
// Created by john on 10/14/16.
//

#include "Box.h"

Box::Box(GLfloat w, GLfloat l, GLfloat h) {
    width = w;
    length = l;
    height = h;

    GLfloat hWidth = width / 2;
    GLfloat hLength = length / 2;
    GLfloat hHeight = height / 2;

    /* Structure:
     * vertices (3), normal (3), color (3)
     */
    GLfloat vertices[] = {
            // Original coordinates
            /*
            hWidth, hHeight, hLength, 1.0f, 0.0f, 0.0f, // 0
            -hWidth, hHeight, hLength, 1.0f, 0.0f, 0.0f, // 1
            -hWidth, -hHeight, hLength, 1.0f, 0.0f, 0.0f, // 2
            hWidth, -hHeight, hLength, 1.0f, 0.0f, 0.0f, // 3
            hWidth, hHeight, -hLength, 1.0f, 0.0f, 0.0f, // 4
            -hWidth, hHeight, -hLength, 1.0f, 0.0f, 0.0f, // 5
            -hWidth, -hHeight, -hLength, 1.0f, 0.0f, 0.0f, // 6
            hWidth, -hHeight, -hLength, 1.0f, 0.0f, 0.0f, // 7
             */

            // back wall = 0
            hWidth, hHeight, hLength, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, // 0
            -hWidth, hHeight, hLength, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, // 1
            -hWidth, -hHeight, hLength, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, // 2
            hWidth, -hHeight, hLength, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, // 3

            // top = 4
            hWidth, hHeight, hLength, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 0
            -hWidth, hHeight, hLength, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 1
            hWidth, hHeight, -hLength, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 4
            -hWidth, hHeight, -hLength, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 5

            // bottom = 8
            -hWidth, -hHeight, hLength, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 2
            hWidth, -hHeight, hLength, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 3
            -hWidth, -hHeight, -hLength, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 6
            hWidth, -hHeight, -hLength, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 7

            // front wall = 12
            hWidth, hHeight, -hLength, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 4
            -hWidth, hHeight, -hLength, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 5
            -hWidth, -hHeight, -hLength, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 6
            hWidth, -hHeight, -hLength, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 7

            // left side = 16
            hWidth, hHeight, hLength, 1.0, 0.0, 0.0, 1.0f, 0.0f, 0.0f, // 0
            hWidth, -hHeight, hLength, 1.0, 0.0, 0.0, 1.0f, 0.0f, 0.0f, // 3
            hWidth, hHeight, -hLength, 1.0, 0.0, 0.0, 1.0f, 0.0f, 0.0f, // 4
            hWidth, -hHeight, -hLength, 1.0, 0.0, 0.0, 1.0f, 0.0f, 0.0f, // 7

            // right side = 20
            -hWidth, hHeight, hLength, -1.0, 0.0, 0.0, 1.0f, 0.0f, 0.0f, // 1
            -hWidth, -hHeight, hLength, -1.0, 0.0, 0.0, 1.0f, 0.0f, 0.0f, // 2
            -hWidth, hHeight, -hLength, -1.0, 0.0, 0.0, 1.0f, 0.0f, 0.0f, // 5
            -hWidth, -hHeight, -hLength, -1.0, 0.0, 0.0, 1.0f, 0.0f, 0.0f, // 6
    };
    GLuint indices[] = {
            // back wall
            0, 1, 2,
            0, 2, 3,
            // top
            7, 5, 4,
            6, 7, 4,
            // bottom
            9, 8, 10,
            9, 10, 11,
            // front wall
            13, 12, 14,
            15, 14, 12,
            // left side
            18, 16, 19,
            19, 16, 17,
            //right side
            20, 22, 21,
            21, 22, 23
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid *) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Box::draw() {
    glFrontFace(GL_CCW);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);
}

