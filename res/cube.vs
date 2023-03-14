#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 normalVector;
out vec3 fragmentPos;
out vec2 textureCoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    gl_Position = u_projection * u_view * u_model * position;
    fragmentPos = vec3(u_model * position);
    normalVector = normal;
    textureCoords = texCoords;
}