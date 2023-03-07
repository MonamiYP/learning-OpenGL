#version 330 core

layout(location = 0) in vec4 position;
// layout(location = 1) in vec4 vertexColor;
layout(location = 1) in vec2 texCoord;

// out vec4 v_vertexColor;
out vec2 v_texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    gl_Position = u_projection * u_view * u_model * position;
    // v_vertexColor = vertexColor;
    v_texCoord = texCoord;
}