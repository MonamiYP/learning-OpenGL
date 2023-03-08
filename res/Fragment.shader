#version 330 core

in vec2 v_texCoord;

layout(location = 0) out vec4 fragment_colour;

uniform sampler2D u_texture;

void main() {
    vec4 texColor = texture(u_texture, v_texCoord);
    fragment_colour = texColor;
}