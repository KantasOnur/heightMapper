#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

out vec3 fragColor;
uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;
uniform float u_time;

void main()
{
    gl_Position = u_projectionMatrix * u_modelViewMatrix * vec4(inPos, 1.0f);
    fragColor = inColor;
}
