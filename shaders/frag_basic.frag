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
    color = vec4(vertexColor, 1.0);

    vec3 colorRev = -1 * diffuseAngle;
    float diffuseDiff = max(dot(vertexNormal, colorRev), 0.0);
    vec3 diffuse = diffuseDiff * diffuseColor;
    diffuse = diffuseStrength * diffuse;

    vec3 ambient = ambientStrength * ambientColor;

    vec3 result = (ambient + diffuse) * vertexColor;
    color = vec4(result, 1.0f);
}