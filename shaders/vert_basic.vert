#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 vertexColor;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main(){
    gl_Position = projection * view * model * vec4(position, 1.0);
    vertexColor = color;
}