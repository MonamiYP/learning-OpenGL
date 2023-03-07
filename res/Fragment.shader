#version 330 core

// in vec4 v_vertexColor;
in vec2 v_texCoord;

layout(location = 0) out vec4 fragment_colour;

uniform float u_multiple;
uniform sampler2D u_texture;

void main() {
    vec4 texColor = texture(u_texture, v_texCoord);
    fragment_colour = texColor * u_multiple;
    // fragment_colour = texColor * vec4(v_vertexColor.xy * u_multiple, v_vertexColor.zw);
}