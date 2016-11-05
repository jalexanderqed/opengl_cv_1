//
// Created by john on 11/04/16.
//

#include "Cone.h"

Cone::Cone(GLfloat r, GLfloat h) {
    /* Structure:
     * vertices (3), normal (3), color (3)
     */
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLuint> indices;

    int divisions = 50;
    GLfloat step = 2 * M_PI / divisions;
    int index = 0;

    glm::vec3 peakPos(0, h, 0);
    glm::vec3 prevNormal;

    // Push center vertex and normal
    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(0);
    normals.push_back(0);
    normals.push_back(-1);
    normals.push_back(0);
    index++;

    for (int theta = 0; theta <= divisions + 1; theta++) {
        GLfloat tAng = theta * step;

        glm::vec3 basePos(r * sin(tAng), 0, r * cos(tAng));
        glm::vec3 baseTangent = glm::cross(basePos, peakPos);
        glm::vec3 currentNormal = glm::cross((peakPos - basePos), baseTangent);
        glm::vec3 peakNormal = (currentNormal + prevNormal) * 0.5f;

        // Push current base position
        vertices.push_back(basePos.x);
        vertices.push_back(basePos.y);
        vertices.push_back(basePos.z);

        // Push copy of peak position
        vertices.push_back(peakPos.x);
        vertices.push_back(peakPos.y);
        vertices.push_back(peakPos.z);

        // Push current normal
        normals.push_back(currentNormal.x);
        normals.push_back(currentNormal.y);
        normals.push_back(currentNormal.z);

        // Push peak normal (average of current and previous)
        normals.push_back(peakNormal.x);
        normals.push_back(peakNormal.y);
        normals.push_back(peakNormal.z);

        // Push point for bottom triangle
        vertices.push_back(basePos.x);
        vertices.push_back(basePos.y);
        vertices.push_back(basePos.z);
        normals.push_back(0);
        normals.push_back(-1);
        normals.push_back(0);

        if (theta > 1) {
            // Push new side triangle (index + 1 is peakPos copy)
            indices.push_back(index + 1);
            indices.push_back(index - 3);
            indices.push_back(index);

            // Push new bottom triangle
            indices.push_back(0);
            indices.push_back(index + 2 - 3);
            indices.push_back(index + 2);
        }
        index += 3;

        prevNormal = currentNormal;
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

void Cone::draw() {
    glFrontFace(GL_CW);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

