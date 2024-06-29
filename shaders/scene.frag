#version 330 core

in vec3 fragColor;

out vec4 colour;
void main()
{
    colour = vec4(fragColor, 1.0f);
}

