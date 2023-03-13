#version 330 core

in vec3 normalVector;
in vec3 fragmentPos;
out vec4 fragmentColour;

uniform vec4 u_cubeColour;
uniform vec4 u_lightColour;
uniform vec3 u_lightPos;

void main() {
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * u_lightColour;

    vec3 normal = normalize(normalVector);
    vec3 lightDirection = normalize(u_lightPos - fragmentPos);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec4 diffuse = diff * u_lightColour;

    fragmentColour = (ambient + diffuse) * u_cubeColour;
}