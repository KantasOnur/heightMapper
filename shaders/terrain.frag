#version 330 core


in vec3 objectColor;
in vec2 texCoord;
in vec3 fragPos;
flat in vec3 fragNormal;

out vec4 color;

uniform sampler2D tex0;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{

/*
    vec3 ambient = 0.1 * vec3(1.0f); // white colored light
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(outNormal, lightDir), 0.0);
    vec3 diffuse = 0.2 * diff * vec3(1.0f); // white colored light

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, outNormal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 30);
    vec3 specular =1 * spec * vec3(1.0f);
*/



    color = vec4(objectColor, 1.0);//vec4(outNormal, 1.0);//texture(tex0, texCoord); // Sample the red channel for single-channel textures
}
