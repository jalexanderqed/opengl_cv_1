#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
    gl_Position = vec4(position, 1.0f);
    gl_Position.x = position.x;
    gl_Position.y = position.y;
    TexCoords = texCoords;
}