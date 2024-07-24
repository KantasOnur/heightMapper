#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 aTex;

out vec3 objectColor;
out vec2 texCoord;
out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;

uniform sampler2D tex0;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float threshold;

vec3 calculateNormal(sampler2D heightMap, vec2 coords)
{
    float heightL = texture(heightMap, coords + vec2(-1.0, 0.0) / 256).r;
    float heightR = texture(heightMap, coords + vec2(1.0, 0.0) / 256).r;
    float heightU = texture(heightMap, coords + vec2(0.0, 1.0) / 256).r;
    float heightD = texture(heightMap, coords + vec2(0.0, -1.0) / 256).r;

    vec3 tangent = normalize(vec3(2.0/256, heightR - heightL, 0.0));
    vec3 bitangent = normalize(vec3(0.0, heightU - heightD, -2.0/256));
    vec3 normal = normalize(cross(tangent, bitangent));
    return normal;
}

float surroudingHeightsSimilarity(sampler2D heightMap, vec2 coords)
{
    float v0 = texture(heightMap, coords + vec2(-1.0, 0.0) / 256).r;
    float v1 = texture(heightMap, coords + vec2(1.0, 0.0) / 256).r;
    float v2 = texture(heightMap, coords + vec2(0.0, 1.0) / 256).r;
    float v3 = texture(heightMap, coords + vec2(0.0, -1.0) / 256).r;
    float v = fragPos.y;

    // If the surrounding heights are similar enough the function return 1.0f
    return (step(abs(v-v0)/v,threshold)+step(abs(v-v1)/v,threshold)+step(abs(v-v2)/v,threshold)+step(abs(v-v3)/v,threshold))/4.0f;
}

void main()
{

    vec3 position = inPos+vec3(0.0f, texture(tex0, aTex).r, 0.0f);
    fragPos = position;

    float similarity = surroudingHeightsSimilarity(tex0, aTex);
    // If the height is similar then its snowy otherwise rocky
    objectColor = mix(vec3(0.432f, 0.321f, 0.248f), vec3(0.88f, 0.96f, 0.89f), step(1.0f,similarity));

    gl_Position = u_projectionMatrix * u_modelViewMatrix * vec4(position, 1.0f);
    texCoord = aTex;
    fragNormal = calculateNormal(tex0, aTex);
}