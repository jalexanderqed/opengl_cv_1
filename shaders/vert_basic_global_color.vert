#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 vertexColor;
out vec3 vertexNormal;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;
uniform mat3 normalModelMat;
uniform vec3 globalColorVar;

void main(){
    gl_Position = projectionMat * viewMat * modelMat * vec4(position, 1.0);
    vertexColor = globalColorVar;
    vertexNormal = normalize(normalModelMat * normal);
}