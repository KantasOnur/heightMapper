#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 aTex;

out vec3 objectColor;
out vec2 texCoord;
out vec3 outNormal;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;

uniform sampler2D tex0;


float getHeight(vec2 tCoord)
{
    return 2*length(texture(tex0, tCoord).rgb);
}

vec3 calculateNormal()
{

    float triangleSide = 10.0f / 400.0f;
    float stepSize = 1.0f / 400.0f;

    vec3 current_vector = vec3(inPos.x, getHeight(aTex), inPos.z);

    vec3 west_vector = inPos + vec3(triangleSide, 0, 0);
    west_vector.y = getHeight(aTex + vec2(stepSize, 0));
    vec3 north_vector = inPos - vec3(0, 0, triangleSide);
    north_vector.y = getHeight(aTex + vec2(0, stepSize));

    // Correcting the vectors
    vec3 toWest = west_vector - current_vector;
    vec3 toNorth = north_vector - current_vector;

    return normalize(cross(toNorth, toWest));
}

void main()
{
    objectColor = inColor;
    gl_Position = u_projectionMatrix * u_modelViewMatrix * vec4(inPos+vec3(0, getHeight(aTex), 0), 1.0f);

    texCoord = aTex;
    outNormal = calculateNormal();
}