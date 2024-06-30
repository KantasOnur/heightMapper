#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;
uniform float u_time;

out vec3 fragNormal;
out vec3 fragPos;
out vec3 objectColor;

const int NUM_WAVES = 4;
/*vec3(tangent at x, y displacement, tangent at z)*/
vec3 wave()
{
    float y_displacement = 0.0f;

    const vec3 wave_directions[NUM_WAVES] = vec3[NUM_WAVES](vec3(1,0,1), vec3(2,0, 6), vec3(4, 0, 1), vec3(3, 0, 3));
    const float a[NUM_WAVES] = float[NUM_WAVES](0.05f, 0.03f, 0.09f, 0.065f);
    const float o[NUM_WAVES] = float[NUM_WAVES](4.0f, 4.3f, 6.9f, 1.3f);

    float dfdx = 0.0f;
    float dfdz = 0.0f;
    for (int i = 0; i < NUM_WAVES; i++)
    {
        float dot_product = dot(wave_directions[i], inPos);
        y_displacement += a[i]*sin(dot_product + o[i]*u_time);
        dfdx += a[i] * wave_directions[i].x * cos(dot_product + o[i]*u_time);
        dfdz += a[i] * wave_directions[i].z * cos(dot_product + o[i]*u_time);
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
