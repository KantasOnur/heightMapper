#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;
uniform float u_time;

out vec3 fragNormal;
out vec3 fragPos;
out vec3 objectColor;

const int NUM_WAVES = 1;

#define pi 3.141592653589793

// Rotate a vec2
vec2 rotate(vec2 vec, float rot)
{
    float s = sin(rot), c = cos(rot);
    return vec2(vec.x*c-vec.y*s, vec.x*s+vec.y*c);
}
vec2 randomVector(vec2 pos)
{
    pos = pos + 0.1;
    float x = dot(pos, vec2(123.4, 234.5));
    float y = dot(pos, vec2(234.5, 345.6));
    vec2 vector = vec2(x,y);
    vector = sin(vector);
    vector = vector * 43758.5453;
    vector = sin(vector);
    return vector;
}
/*vec3(tangent at x, y displacement, tangent at z)*/
vec3 wave()
{

    const float o[4] = float[4](4.0f, 1.5f, 7.0f, 3.0f);
    const float a[4] = float[4](0.5f, 1.0f, 0.6, 0.069f);
    float dfdx = 0.0f;
    float dfdz = 0.0f;

    float amplitude = 0.82;
    float frequency = 1.12;

    float y_displacement = 0.0f;
    vec2 direction = normalize(vec2(1, 0));

    for (int i = 0; i < 32; i++)
    {
        float new_amplitude = 0.2*pow(amplitude, i);
        float new_frequency = pow(frequency, i);
        float inside_sin = new_frequency*dot(direction, vec2(inPos.xz)-vec2(dfdx,dfdz))+u_time*o[i%4];

        y_displacement += exp(new_amplitude * sin(inside_sin)) - 1;
        dfdx += new_amplitude*cos(inside_sin)*new_frequency*direction.x * (y_displacement+1);
        dfdz += new_amplitude*cos(inside_sin)*new_frequency*direction.y * (y_displacement+1);

        direction = normalize(rotate(direction, 0.125*pi)+direction);
        /*
        float new_amplitude = 0.12 * pow(amplitude, i);
        float new_frequency = pow(frequency, i);
        float inside_sin = dot(direction*new_frequency, vec2(inPos.xz)) + u_time*o[i%4];

        y_displacement += exp(new_amplitude * sin(inside_sin)) - 1;
        dfdx += new_amplitude*cos(inside_sin)*direction.x*new_frequency * (y_displacement+1); //+1 since one is a constant, so dont want to include in the derivative
        dfdz += new_amplitude*cos(inside_sin)*direction.y*new_frequency * (y_displacement+1);
        direction = normalize(rotate(direction, 0.125*pi));
        */
    }
    return vec3(dfdx, y_displacement, dfdz);
}

void main()
{
    vec3 wave = wave(); // Assume wave() returns vec3(dfdx, y_displacement, dfdz)
    // Cross product to get the normal vector
    //vec3 normal = normalize(cross(tangent1, tangent2));

    vec3 position = vec3(inPos.x, wave.y, inPos.z);

    fragNormal = normalize(cross(vec3(0, wave.z, 1.0f), vec3(1.0f, wave.x, 0.0f)));
    fragPos = position;
    objectColor = inColor;

    gl_Position = u_projectionMatrix * u_modelViewMatrix * vec4(position, 1.0f);
}