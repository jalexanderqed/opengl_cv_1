#ifndef GLHW_01_UTIL_H
#define GLHW_01_UTIL_H

#include <fstream>
#include <string>

using namespace std;

string readFile(string name) {
    ifstream ifs(name);
    string content((istreambuf_iterator<char>(ifs)),
                   (istreambuf_iterator<char>()));
    return content;
}

enum vertex_shaders {
    BASIC_VERTEX_SHADER = 0,
    VERTEX_SHADER_MAX
};

enum fragment_shaders {
    BASIC_FRAGMENT_SHADER = VERTEX_SHADER_MAX,
    FRAGMENT_SHADER_MAX
};

#endif