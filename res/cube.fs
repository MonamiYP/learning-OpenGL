#version 330 core

in vec3 normalVector;
in vec3 fragmentPos;

out vec4 fragmentColour;

uniform vec3 u_cubeColour;
uniform vec3 u_lightColour;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

void main() {
    // Ambient light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColour;

    // Diffuse light
    vec3 normal = normalize(normalVector);
    vec3 lightDirection = normalize(u_lightPos - fragmentPos);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * u_lightColour;

    // Specular light
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_viewPos - fragmentPos);
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_lightColour;

    fragmentColour = vec4((ambient + diffuse + specular) * u_cubeColour, 1.0);
}