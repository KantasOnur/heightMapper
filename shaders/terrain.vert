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



vec3 calculateNormal(sampler2D heightMap, vec2 coords)
{
    float heightL = texture(heightMap, coords + vec2(-1.0, 0.0) / 512).r;
    float heightR = texture(heightMap, coords + vec2(1.0, 0.0) / 512).r;
    float heightU = texture(heightMap, coords + vec2(0.0, 1.0) / 512).r;
    float heightD = texture(heightMap, coords + vec2(0.0, -1.0) / 512).r;

    vec3 tangentX = normalize(vec3(2.0 / 512, 0.0, (heightL - heightR)));
    vec3 tangentY = normalize(vec3(0.0, 2.0 / 512, (heightU - heightD)));

    /*trial and error*/
    vec3 tangent = normalize(vec3(2.0/512, heightR - heightL, 0.0));
    vec3 bitangent = normalize(vec3(0.0, heightD - heightU, 2.0/512));
    vec3 normal = normalize(cross(bitangent, tangent));
    return normal;
}

void main()
{

    objectColor = inColor;
    vec3 position = inPos+vec3(0, texture(tex0, aTex).r, 0);
    gl_Position = u_projectionMatrix * u_modelViewMatrix * vec4(position, 1.0f);
    texCoord = aTex;
    fragNormal = calculateNormal(tex0, aTex);
    fragPos = position;


    //objectColor = specular + diffuse;
}