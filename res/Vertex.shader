#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 texCoord;

out vec4 v_vertexColor;
out vec2 v_texCoord;

uniform mat4 transform;

void main() {
    gl_Position = transform * position;
    v_vertexColor = vertexColor;
    v_texCoord = texCoord;
}