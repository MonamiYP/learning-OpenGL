#version 330 core

in vec3 normalVector;
in vec3 fragmentPos;

out vec4 fragmentColour;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 u_viewPos;

void main() {
    // Ambient light
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse light
    vec3 normal = normalize(normalVector);
    vec3 lightDirection = normalize(light.position - fragmentPos);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular light
    vec3 viewDir = normalize(u_viewPos - fragmentPos);
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    fragmentColour = vec4((ambient + diffuse + specular), 1.0);
}