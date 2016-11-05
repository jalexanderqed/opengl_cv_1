//
// Created by john on 11/04/16.
//

#include "Torus.h"

Torus::Torus(GLfloat r1, GLfloat r2) {
    /* Structure:
     * vertices (3), normal (3), color (3)
     */
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLuint> indices;

    int divisions = 20;
    GLfloat step = M_PI / divisions;
    int index = 0;

    for(int theta = 0; theta <= divisions * 2; theta++){
        GLfloat tAng = theta * step;
        GLfloat rimX = r1 * sin(tAng);
        GLfloat rimY = 0;
        GLfloat rimZ = r1 * cos(tAng);
        for(int phi = 0; phi <= divisions * 2; phi++){
            GLfloat pAng = phi * step;

            GLfloat x = r2 * sin(tAng) * cos(pAng) + rimX;
            GLfloat y = r2 * sin(pAng) + rimY;
            GLfloat z = r2 * cos(tAng) * cos(pAng) + rimZ;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            normals.push_back(x - rimX);
            normals.push_back(y - rimY);
            normals.push_back(z - rimZ);

            if(index >= 2 * divisions + 2) {
                indices.push_back(index);
                indices.push_back(index - 1);
                indices.push_back(index - 2 * divisions - 2);

                indices.push_back(index);
                indices.push_back(index - 2 * divisions - 2);
                indices.push_back(index - 2 * divisions - 1);
            }

            index++;
        }
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    GLuint NBO;
    glGenBuffers(1, &NBO);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(1);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    numIndices = indices.size();

    glBindVertexArray(0);
}

void Torus::draw() {
    glFrontFace(GL_CCW);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

