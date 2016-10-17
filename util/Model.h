//
// Created by john on 10/16/16.
//

#ifndef GLHW_01_MODEL_H
#define GLHW_01_MODEL_H


#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/soil/inc/SOIL/SOIL.h"

#include "Mesh.h"
#include "../lib/assimp-3.3.1/include/assimp/Importer.hpp"
#include "../lib/assimp-3.3.1/include/assimp/postprocess.h"
#include "../lib/assimp-3.3.1/include/assimp/scene.h"

GLint TextureFromFile(const char* path, string directory);

class Model
{
public:
    /*  Functions   */
    // Constructor, expects a filepath to a 3D model.
    Model(string path)
    {
        this->loadModel(path);
    }

    Model() { }

    // Draws the model, and thus all its meshes
    void Draw(ShaderProgram shader)
    {
        for(GLuint i = 0; i < this->meshes.size(); i++)
            this->meshes[i].Draw(shader);
    }

private:
    /*  Model Data  */
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

    /*  Functions   */
    // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string path);

    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // Checks all material textures of a given type and loads the textures if they're not loaded yet.
    // The required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif //GLHW_01_MODEL_H
