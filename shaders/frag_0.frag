#version 330 core

uniform vec3 lightColor;

in vec3 vertexColor;
out vec4 color;

void main(){
    color = vec4(lightColor * vertexColor, 1.0);
}