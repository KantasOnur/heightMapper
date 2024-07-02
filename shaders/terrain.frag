#version 330 core

in vec3 objectColor;
in vec3 outNormal;

out vec4 color;

in vec2 texCoord;
uniform sampler2D tex0;
void main()
{
    color = vec4(outNormal, 1.0);//texture(tex0, texCoord); // Sample the red channel for single-channel textures
}
