#version 330 core

in vec3 vertexColor;
in vec3 vertexNormal;

out vec4 color;

uniform vec3 diffuseAngle;
uniform vec3 diffuseColor;
uniform float diffuseStrength;
uniform float ambientStrength;
uniform vec3 ambientColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

void main(){
    color = vec4(vertexColor, 1.0f);
}