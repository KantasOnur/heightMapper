#version 330 core

in vec3 fragNormal; //already normalized
in vec3 fragPos;
in vec3 objectColor;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{

    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    vec3 ambient = ambientStrength * vec3(1.0f); // white colored light

    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(fragNormal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0f); // white colored light

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fragNormal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 50);
    vec3 specular = specularStrength * spec * vec3(1.0f);

    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0f);
}

