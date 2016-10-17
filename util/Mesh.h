//
// Created by john on 10/16/16.
// Taken from http://www.learnopengl.com/code_viewer.php?code=mesh&type=header
//

#ifndef GLHW_01_MESH_H
#define GLHW_01_MESH_H


#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/assimp-3.3.1/include/assimp/types.h"
#include "ShaderProgram.h"
#include "util.h"

class Mesh {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    /*  Functions  */
    // Constructor
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);

    // Render the mesh
    void Draw(ShaderProgram shader);

private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;

    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void setupMesh();
};

#endif //GLHW_01_MESH_H
