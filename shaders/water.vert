#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTextureCoord;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelViewMatrix;
uniform float u_time;
uniform sampler2D tex0;

out vec3 fragPos;
out vec3 fragColor;
out vec2 fragTextureCoord;
flat out vec3 fragNormal;

#define pi 3.141592653589793


float getHeight(vec2 tCoord)
{
    return 8*(pow(texture(tex0, tCoord).r, 4.5));
}

vec2 rotate(vec2 vec, float rot)
{
    float s = sin(rot), c = cos(rot);
    return vec2(vec.x*c-vec.y*s, vec.x*s+vec.y*c);
}

vec3 wave()
{

    const float o[4] = float[4](4.0f, 1.5f, 7.0f, 3.0f);
    const float a[4] = float[4](0.5f, 1.0f, 0.6, 0.069f);
    float dfdx = 0.0f;
    float dfdz = 0.0f;

    float amplitude = 0.7;
    float frequency = 1.3;

    float y_displacement = 0.0f;
    vec2 direction = normalize(vec2(0, -1));

    for (int i = 0; i < 10; i++)
    {
        float new_amplitude = 0.2 * pow(amplitude, i);
        float new_frequency = pow(frequency, i);
        vec2 posXZ = vec2(inPos.xz) - 1* vec2(dfdz, dfdz);
        float inside_sin = new_frequency * dot(direction, posXZ) + u_time * o[i % 4];

        y_displacement += min(max(exp(new_amplitude * sin(inside_sin)) - 1.0, 0.0), 1.5);

        float cos_term = new_amplitude * cos(inside_sin) * new_frequency;
        dfdx += cos_term * direction.x * (y_displacement + 1.0);
        dfdz += cos_term * direction.y * (y_displacement + 1.0);


        direction = normalize(rotate(direction,0.125*pi) - direction);
    }
    return vec3(dfdx, y_displacement, dfdz);
}

void main()
{
    float height = -getHeight(inTextureCoord);
    float adjustedHeight = -height * step(-0.2, height);
    vec3 position = inPos + vec3(0, adjustedHeight, 0);

    vec3 wave = wave();

    float stepResult = step(0.001, position.y);
    position.y += wave.y * stepResult;

    gl_Position = u_projectionMatrix * u_modelViewMatrix * vec4(position, 1.0f);
    fragColor = inColor;
    fragTextureCoord = inTextureCoord;
    fragPos = position;
    fragNormal = normalize(cross(vec3(0, wave.z, 1.0f), vec3(1.0f, wave.x, 0.0f)));
}