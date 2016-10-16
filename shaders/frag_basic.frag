#version 330 core

in vec3 vertexColor;
in vec3 vertexNormal;

out vec4 color;

uniform vec3 diffuseAngle;
uniform vec3 diffuseColor;
uniform float ambientStrength;
uniform vec3 ambientColor;

void main(){
    color = vec4(vertexColor, 1.0);

    vec3 colorRev = -1 * diffuseAngle;
    float diffuseDiff = max(dot(vertexNormal, colorRev), 0.0);
    vec3 diffuse = diffuseDiff * diffuseColor;

    vec3 ambient = ambientStrength * ambientColor;

    vec3 result = (ambient + diffuse) * vertexColor;
    color = vec4(result, 1.0f);
}