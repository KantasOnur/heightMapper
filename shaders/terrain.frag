#version 330 core


in vec3 objectColor;
in vec2 texCoord;
in vec3 fragPos;
in vec3 fragNormal;

out vec4 color;

uniform sampler2D tex0;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{


    vec3 ambient = 0.1 * vec3(1.0f); // white colored light
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(fragNormal, lightDir), 0.0);
    vec3 diffuse = 0.2 * diff * vec3(1.0f); // white colored light

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 30);
    vec3 specular = 0.06* spec * vec3(1.0f);


    color = vec4((ambient + diffuse + specular), 1.0);
}
