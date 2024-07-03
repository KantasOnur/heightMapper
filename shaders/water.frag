#version 330 core

in vec3 fragNormal; //already normalized
in vec3 fragPos;
in vec3 objectColor;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{

    float ambientStrength = 0.5;
    float specularStrength = 2.0;

    vec3 ambient = ambientStrength * vec3(1.0f); // white colored light

    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(fragNormal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0f); // white colored light

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fragNormal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 30);
    vec3 specular = specularStrength * spec * vec3(1.0f);


    float scatterDistance = length(fragPos - lightPos); // Simplified distance
    vec3 scatterAttenuation = exp(-vec3(0.2f) * scatterDistance);

    vec3 exitPoint = fragPos + fragNormal * scatterDistance;
    vec3 exitDirection = normalize(exitPoint - fragPos);

    vec3 scatteredLight = scatterAttenuation * exitDirection;


    vec3 result = (ambient + scatteredLight + specular) * objectColor;
    color = vec4(result, 1.0f);

}

