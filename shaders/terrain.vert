#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 aTex;

out vec3 objectColor;
out vec2 texCoord;
out vec3 outNormal;
out vec3 fragPos;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;

uniform sampler2D tex0;
uniform float triangleSide;
uniform float stepSize;


float getHeight(vec2 tCoord)
{
    return 10*(texture(tex0, tCoord).r);
}


vec3 calculateNormal()
{
    float triangleSide = 10.0f / 512.0f;
    float stepSize = 1.0f / 512.0f;

    vec3 current_vector = inPos + vec3(0, getHeight(aTex), 0);

    vec3 west_vector = inPos + vec3(triangleSide, 0, 0);
    west_vector.y = getHeight(aTex + vec2(stepSize, 0));
    vec3 east_vector = inPos - vec3(triangleSide, 0, 0);
    east_vector.y = getHeight(aTex - vec2(stepSize, 0));
    vec3 north_vector = inPos - vec3(0, 0, triangleSide);
    north_vector.y = getHeight(aTex + vec2(0, stepSize));
    vec3 south_vector = inPos + vec3(0, 0, triangleSide);
    south_vector.y = getHeight(aTex - vec2(0, stepSize));

    // Correcting the vectors
    vec3 toWest = west_vector - current_vector;
    vec3 toEast = east_vector - current_vector;
    vec3 toNorth = north_vector - current_vector;
    vec3 toSouth = south_vector - current_vector;

    // Calculating normals for the surrounding triangles
    vec3 normal1 = cross(toWest, toNorth);
    vec3 normal2 = cross(toNorth, toEast);
    vec3 normal3 = cross(toEast, toSouth);
    vec3 normal4 = cross(toSouth, toWest);

    // Averaging the normals to get a smooth normal
    vec3 smoothNormal = normalize(normal1 + normal2 + normal3 + normal4);

    return smoothNormal;
}

void main()
{

    objectColor = inColor;
    vec3 position = inPos+vec3(0, getHeight(aTex)-0.05, 0);
    gl_Position = u_projectionMatrix * u_modelViewMatrix * vec4(position, 1.0f);
    fragPos = position;

    texCoord = aTex;
    outNormal = calculateNormal();
}