#define GLEW_STATIC
#include <GL/glew.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <sstream>

#include "util/util.h"

using namespace std;

string shaderStrings[fragment_shaders::FRAGMENT_SHADER_MAX];
GLuint shaders[fragment_shaders::FRAGMENT_SHADER_MAX];

GLuint basicProgram;

GLuint triangleVAO;

void draw(){
    glUseProgram(basicProgram);
    glBindVertexArray(triangleVAO);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void initObjects(){
    glGenVertexArrays(1, &triangleVAO);

    glBindVertexArray(triangleVAO);
    GLfloat vertices[] = {
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
    };
    GLuint indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void loadShaders(){
    for(int i = 0; i < vertex_shaders::VERTEX_SHADER_MAX; i++){
        ostringstream fileName;
        fileName << "shaders/vert_" << i << ".vert";
        shaderStrings[i] = (readFile(fileName.str()));
    }

    for(int i = vertex_shaders::VERTEX_SHADER_MAX; i < fragment_shaders::FRAGMENT_SHADER_MAX; i++){
        ostringstream fileName;
        int shaderNum = i - vertex_shaders::VERTEX_SHADER_MAX;
        fileName << "shaders/frag_" << shaderNum << ".frag";
        shaderStrings[i] = (readFile(fileName.str()));
    }
}

int compileShaders(){
    for(int i = 0; i < vertex_shaders::VERTEX_SHADER_MAX; i++){
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* cstr = shaderStrings[i].c_str();
        glShaderSource(vertexShader, 1, &cstr, NULL);
        glCompileShader(vertexShader);
        GLint success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success){
            GLchar infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED index:"
                 << i << "\n" << infoLog << endl;
            cout << shaderStrings[i] << endl;
            return -1;
        }
        shaders[i] = vertexShader;
    }

    for(int i = vertex_shaders::VERTEX_SHADER_MAX; i < fragment_shaders::FRAGMENT_SHADER_MAX; i++){
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* cstr = shaderStrings[i].c_str();
        glShaderSource(fragmentShader, 1, &cstr, NULL);
        glCompileShader(fragmentShader);
        GLint success;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success){
            GLchar infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED index:"
                 << i << "\n" << infoLog << endl;
            cout << shaderStrings[i] << endl;
            return -1;
        }
        shaders[i] = fragmentShader;
    }
    return 0;
}

int createPrograms(){
    basicProgram = glCreateProgram();
    glAttachShader(basicProgram, shaders[vertex_shaders::BASIC_VERTEX_SHADER]);
    glAttachShader(basicProgram, shaders[fragment_shaders::BASIC_FRAGMENT_SHADER]);
    glLinkProgram(basicProgram);

    GLint success;
    glGetProgramiv(basicProgram, GL_LINK_STATUS, &success);
    if(!success){
        GLchar infoLog[512];
        glGetShaderInfoLog(basicProgram, 512, NULL, infoLog);
        cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        return -1;
    }
    return 0;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "glhw_01", nullptr, nullptr);
    if(window == nullptr){
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }

    loadShaders();
    if(compileShaders() != 0){
        return -1;
    }

    if(createPrograms() != 0){
        return -1;
    }
    initObjects();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSetKeyCallback(window, keyCallback);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        draw();
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}