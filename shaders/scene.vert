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

/*vec3(tangent at x, y displacement, tangent at z)*/
vec3 wave()
{

    const float o[NUM_WAVES] = float[NUM_WAVES](4.0f);
    /*
    const float a[NUM_WAVES] = float[NUM_WAVES](0.2f, 0.5f, 0.3f, 0.65f, 0.5);
    const float o[NUM_WAVES] = float[NUM_WAVES](4.0f, 4.3f, 6.9f, 1.3f, 6.9f);
*/
    float dfdx = 0.0f;
    float dfdz = 0.0f;

    float amplitude = 0.82;
    float frequency = 1.12;

    float y_displacement = 0.0f;
    vec2 direction = normalize(vec2(cos(0.5), sin(0.5)));

    for (int i = 0; i < 32; i++)
    {

        float new_amplitude = 0.1*pow(amplitude, i);
        float new_frequency = pow(frequency, i);
        float inside_sin = dot(direction*new_frequency, vec2(inPos.xz))*frequency + u_time*o[i];

        y_displacement += exp(new_amplitude * sin(inside_sin)) - 1;
        dfdx += new_amplitude*cos(inside_sin)*direction.x*new_frequency * (y_displacement+1); //+1 since one is a constant, so dont want to include in the derivative
        dfdz += new_amplitude*cos(inside_sin)*direction.y*new_frequency * (y_displacement+1);

        direction = normalize(rotate(direction, 0.125*pi));
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
