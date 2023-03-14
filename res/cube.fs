#version 330 core

in vec3 normalVector;
in vec3 fragmentPos;
in vec2 textureCoords;

out vec4 fragmentColour;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureCoords));

    // Diffuse light
    vec3 normal = normalize(normalVector);
    vec3 lightDirection = normalize(light.position - fragmentPos);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, textureCoords));

    // Specular light
    vec3 viewDir = normalize(u_viewPos - fragmentPos);
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, textureCoords));

    fragmentColour = vec4((ambient + diffuse + specular), 1.0);
}