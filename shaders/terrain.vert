#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 uvCoords;

out vec3 objectColor;
out vec2 objectUv;
uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;

void main()
{
    objectColor = inColor;
    gl_Position = u_projectionMatrix * u_modelViewMatrix * vec4(inPos, 1.0f);
    objectUv = uvCoords;
}