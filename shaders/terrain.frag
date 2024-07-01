#version 330 core

in vec3 objectColor;
in vec2 objectUv;
out vec4 color;

void main()
{
    color = vec4(objectUv, 0.0, 1.0f);
}
