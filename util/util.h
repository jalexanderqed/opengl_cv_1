#ifndef GLHW_01_UTIL_H
#define GLHW_01_UTIL_H

#include <fstream>
#include <string>
#include <GL/glew.h>
#include "../lib/glm/vec2.hpp"
#include "../lib/assimp-3.3.1/include/assimp/types.h"
#include "../lib/glm/vec3.hpp"
#include "../lib/soil/inc/SOIL/SOIL.h"

using namespace std;

struct Vertex {
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // TexCoords
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    string type;
    aiString path;
};

string myReadFile(string name);

GLint TextureFromFile(const char* path, string directory);

enum vertex_shaders {
    BASIC_VERTEX_SHADER = 0,
    MODEL_VERTEX_SHADER,
    VERTEX_SHADER_MAX
};

enum fragment_shaders {
    BASIC_FRAGMENT_SHADER = VERTEX_SHADER_MAX,
    MODEL_FRAGMENT_SHADER,
    FRAGMENT_SHADER_MAX
};

#endif