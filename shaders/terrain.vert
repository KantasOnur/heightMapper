#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 aTex;

out vec3 objectColor;
out vec2 texCoord;
flat out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;

uniform sampler2D tex0;
uniform float triangleSide;
uniform float stepSize;

uniform vec3 lightPos;
uniform vec3 viewPos;

float getHeight(vec2 tCoord)
{
    float u = 8*pow(texture(tex0, tCoord).r, 4.3);
    float v = 8*pow(texture(tex0, tCoord + vec2(stepSize, 0)).r, 4.3);
    float w = 8*pow(texture(tex0, tCoord + vec2(0, stepSize)).r, 4.3);
    return texture(tex0, tCoord).r/0.5 - 1;
}


vec3 calculateNormal(vec3 p1, vec3 p2, vec3 p3)
{

    vec3 u = p2 - p1;
    vec3 v = p3 - p1;
    vec3 normal = normalize(cross(u, v));
    return normal;
}

void main()
{

    objectColor = inColor;
    vec3 position = inPos+vec3(0, getHeight(aTex), 0);
    gl_Position = u_projectionMatrix * u_modelViewMatrix * vec4(position, 1.0f);

    texCoord = aTex;
    vec3 p1 = position;
    vec3 p2 = vec3(inPos.x + triangleSide, getHeight(aTex + vec2(stepSize, 0)), inPos.z);
    vec3 p3 = vec3(inPos.x, getHeight(aTex + vec2(0, stepSize)), -triangleSide + inPos.z);

    fragNormal = calculateNormal(p1, p2, p3);
    fragPos = position;

    vec3 ambient = 0.1 * vec3(1.0f); // white colored light
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(fragNormal, lightDir), 0.0);
    vec3 diffuse = 0.2 * diff * vec3(1.0f); // white colored light

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 30);
    vec3 specular = 0.06* spec * vec3(1.0f);

    objectColor = specular + diffuse;
}